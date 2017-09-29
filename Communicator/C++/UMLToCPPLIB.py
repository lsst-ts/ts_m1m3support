
import sys
import xml.etree.ElementTree

ignoreGlobals = True
globalCommands = ["Start", "Enable", "Disable", "Standby"]
globalEvents = ["ErrorCode", "SummaryState", "SettingVersions", "AppliedSettingsMatchStart", "SettingsApplied"]

class UMLParser:
    def Open(self, subsystem, umlFile):
        tempUMLFile = umlFile + ".tmp"
        with open(tempUMLFile, "w") as tempFile:
            with open(umlFile, "r") as inputFile:
                for line in inputFile:
                    tempFile.write(line.replace("<UML:", "<").replace("</UML:", "</"))

        self.subsystem = subsystem
        self.uml = xml.etree.ElementTree.parse(tempUMLFile)
        
    def Parse(self, outputDirectory):
        self.outputDirectory = outputDirectory
        file = SALFile(self.subsystem, self.GetTelemetry(), self.GetEvents(), self.GetCommands())
        header, cpp, make = file.Code()
        with open("%s\\libSAL%s.h" % (self.outputDirectory, self.subsystem), "w") as output:
            output.write(header)
        with open("%s\\libSAL%s.cpp" % (self.outputDirectory, self.subsystem), "w") as output:
            output.write(cpp)
        with open("%s\\Makefile.libSAL%s" % (self.outputDirectory, self.subsystem), "w") as output:
            output.write(make)
            
        
    def GetCommands(self):
        commands = []
        for item in self.GetCommandList():
            command = self.CreateSALCommand(item)
            if command.name != "Command":
                commands.append(command)
        return commands
        
    def GetEvents(self):
        events = []
        for item in self.GetEventList():
            event = self.CreateSALEvent(item)
            if event.name != "Event":
                events.append(event)
        return events
        
    def GetTelemetry(self):
        telemetry = []
        for item in self.GetTelemetryList():
            tlm = self.CreateSALTelemetry(item)
            if tlm.name != "Telemetry":
                telemetry.append(tlm)
        return telemetry
        
    def CreateSALCommand(self, command):
        basePath = ".//Package[@name='Command']/Namespace.ownedElement/Class[@name='%s']/ModelElement.taggedValue/TaggedValue[@tag='%s']" % (command, "%s")
        author = self.GetValue(self.uml.find(basePath % "author"), "UNDEFINED")
        parameters = []
        for parameter in self.GetCommandParameterList(command):
            parameters.append(self.CreateSALParameter("Command", command, parameter))
        return SALCommand(self.subsystem, "", author, command, parameters)
        
    def CreateSALEvent(self, event):
        basePath = ".//Package[@name='Event']/Namespace.ownedElement/Class[@name='%s']/ModelElement.taggedValue/TaggedValue[@tag='%s']" % (event, "%s")
        author = self.GetValue(self.uml.find(basePath % "author"), "UNDEFINED")
        parameters = []
        for parameter in self.GetEventParameterList(event):
            parameters.append(self.CreateSALParameter("Event", event, parameter))
        return SALEvent(self.subsystem, "", author, event, parameters)
        
    def CreateSALTelemetry(self, telemetry):
        basePath = ".//Package[@name='Telemetry']/Namespace.ownedElement/Class[@name='%s']/ModelElement.taggedValue/TaggedValue[@tag='%s']" % (telemetry, "%s")
        author = self.GetValue(self.uml.find(basePath % "author"), "UNDEFINED")
        parameters = []
        for parameter in self.GetTelemetryParameterList(telemetry):
            parameters.append(self.CreateSALParameter("Telemetry", telemetry, parameter))
        return SALTelemetry(self.subsystem, "", author, telemetry, parameters)

    def CreateSALParameter(self, type, command, parameter):
        basePath = ".//Package[@name='%s']/Namespace.ownedElement/Class[@name='%s']/Classifier.feature/Attribute[@name='%s']/ModelElement.taggedValue/TaggedValue[@tag='%s']" % (type, command, parameter, "%s")
        description = self.GetValue(self.uml.find(basePath % "description"), "")
        type = self.GetValue(self.uml.find(basePath % "type"), "UNDEFINED")
        units = self.GetValue(self.uml.find(basePath % "unit"), "")
        count = self.GetValue(self.uml.find(basePath % "upperBound"), "UNDEFINED")
        return SALParameter(parameter, description, type, units, count)
        
    def GetCommandList(self):
        return [command.get("name") for command in self.uml.findall(".//Package[@name='Command']/Namespace.ownedElement/Class")]
  
    def GetCommandParameterList(self, command):
        return [parameter.get("name") for parameter in self.uml.findall(".//Package[@name='Command']/Namespace.ownedElement/Class[@name='%s']/Classifier.feature/Attribute" % command)]

    def GetEventList(self):
        return [event.get("name") for event in self.uml.findall(".//Package[@name='Event']/Namespace.ownedElement/Class")]
        
    def GetEventParameterList(self, event):
        return [parameter.get("name") for parameter in self.uml.findall(".//Package[@name='Event']/Namespace.ownedElement/Class[@name='%s']/Classifier.feature/Attribute" % event)]
                
    def GetTelemetryList(self):
        return [telemetry.get("name") for telemetry in self.uml.findall(".//Package[@name='Telemetry']/Namespace.ownedElement/Class")]
        
    def GetTelemetryParameterList(self, telemetry):
        return [parameter.get("name") for parameter in self.uml.findall(".//Package[@name='Telemetry']/Namespace.ownedElement/Class[@name='%s']/Classifier.feature/Attribute" % telemetry)]      

    def GetValue(self, node, default):
        return node.get("value") if node is not None else default

class SALFile:
    defineSALObj = "SAL_%s %sSAL = SAL_%s();"
    SALObj = "%sSAL"
    headerCode = """
#ifndef _LIB_SAL_%s_H_
#define _LIB_SAL_%s_H_

typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
typedef unsigned char bool_t;

%s

extern "C" {
extern void libInitializeSAL();
extern int32_t libSALTelemetryPub(const char* topic);
extern int32_t libSALTelemetrySub(const char* topic);
extern int32_t libSALProcessor(const char* topic);
extern int32_t libSALCommand(const char* topic);
extern int32_t libSALEvent(const char* topic);
extern void libShutdownSAL();

%s

}

#endif
"""
    cppCode = """
#include "SAL_%s.h"
#include "ccpp_sal_%s.h"
#include "libSAL%s.h"

SAL_%s %sSAL = SAL_%s();

extern "C" void libInitializeSAL() { %sSAL = SAL_%s(); }
extern "C" int32_t libSALTelemetryPub(const char* topic) { return %sSAL.salTelemetryPub((char*)topic); }
extern "C" int32_t libSALTelemetrySub(const char* topic) { return %sSAL.salTelemetrySub((char*)topic); }
extern "C" int32_t libSALProcessor(const char* topic) { return %sSAL.salProcessor((char*)topic); }
extern "C" int32_t libSALCommand(const char* topic) { return %sSAL.salCommand((char*)topic); }
extern "C" int32_t libSALEvent(const char* topic) { return %sSAL.salEvent((char*)topic); }
extern "C" void libShutdownSAL() { %sSAL.salShutdown(); }
%s
"""
    makeFile = """
#----------------------------------------------------------------------------
#       Macros
#----------------------------------------------------------------------------
CFG = Release

ifeq ($(CFG), Release)
CC            = gcc
CXX           = g++
LD            = $(CXX) $(CCFLAGS) $(CPPFLAGS)
AR            = ar
PICFLAGS      = -fPIC
CPPFLAGS      = $(PICFLAGS) $(GENFLAGS) -shared -g $(SAL_CPPFLAGS) -D_REENTRANT -Wall -I"." -I"$(OSPL_HOME)/examples/include" -I"$(OSPL_HOME)/examples" -I"$(OSPL_HOME)/include" -I"$(OSPL_HOME)/include/sys" -I"$(OSPL_HOME)/include/dcps/C++/SACPP" -I../../%s/cpp/src -I"$(SAL_HOME)/include" -I.. -I"$(SAL_WORK_DIR)/include" -Wno-write-strings #-DSAL_SUBSYSTEM_ID_IS_KEYED
OBJEXT        = .o
OUTPUT_OPTION = -o "$@"
COMPILE.c     = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc    = $(CXX) $(CCFLAGS) $(CPPFLAGS) -c
LDFLAGS       = -L"." -L"$(OSPL_HOME)/lib" -Wl,-rpath,\$$ORIGIN -Wl,-rpath,\$$ORIGIN/$(OSPL_HOME)/lib -L"$(SAL_HOME)/lib"
CCC           = $(CXX)
MAKEFILE      = Makefile.libSAL%s
DEPENDENCIES  = 
BTARGETDIR    = ./
BIN1          = $(BTARGETDIR)libSAL%s.so
OBJS1         = .obj/CheckStatus.o .obj/SAL_%s.o .obj/libSAL%s.o


SRC           = ../src/CheckStatus.cpp ../src/SAL_%s.cpp libSAL%s.cpp
## INSERT COMMANDS TEST SRC
CAT           = cat
MV            = mv -f
RM            = rm -rf
CP            = cp -p
NUL           = /dev/null
MKDIR         = mkdir -p
TESTDIRSTART  = test -d
TESTDIREND    = ||
TOUCH         = touch
EXEEXT        = 
LIBPREFIX     = lib
LIBSUFFIX     = 
GENFLAGS      = -g
LDLIBS        = -l"sacpp_%s_types$(LIBSUFFIX)" -l"dcpssacpp" -l"dcpsgapi" -l"ddsuser" -l"ddskernel" -l"ddsserialization" -l"ddsconfparser" -l"ddsconf" -l"ddsdatabase" -l"ddsutil" -l"ddsos" -ldl $(subst lib,-l,$(sort $(basename $(notdir $(wildcard /usr/lib/librt.so /lib/librt.so))))) -lpthread
LINK.cc       = $(LD) $(LDFLAGS)
EXPORTFLAGS   = 
endif
ifeq ($(CFG), Debug)
CC            = gcc
CXX           = g++
LD            = $(CXX) $(CCFLAGS) $(CPPFLAGS)
AR            = ar
PICFLAGS      = -fPIC
CPPFLAGS      = $(PICFLAGS) $(GENFLAGS) $(SAL_CPPFLAGS) -D_REENTRANT -Wall -I"." -I"$(OSPL_HOME)/examples/include" -I"$(OSPL_HOME)/examples" -I"$(OSPL_HOME)/include" -I"$(OSPL_HOME)/include/sys" -I"$(OSPL_HOME)/include/dcps/C++/SACPP"
OBJEXT        = .o
OUTPUT_OPTION = -o "$@"
COMPILE.c     = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc    = $(CXX) $(CCFLAGS) $(CPPFLAGS) -c
LDFLAGS       = -L"." -L"$(OSPL_HOME)/lib" -Wl,-rpath,\$$ORIGIN -Wl,-rpath,\$$ORIGIN/$(OSPL_HOME)/lib
CCC           = $(CXX)
MAKEFILE      = Makefile.libSAL%s
DEPENDENCIES  =
BTARGETDIR    = ./
CAT           = cat
MV            = mv -f
RM            = rm -rf
CP            = cp -p
NUL           = /dev/null
MKDIR         = mkdir -p
TESTDIRSTART  = test -d
TESTDIREND    = ||
TOUCH         = touch
EXEEXT        = 
LIBPREFIX     = lib
LIBSUFFIX     = 
GENFLAGS      = -g
LDLIBS        = -l"sacpp_%s_types$(LIBSUFFIX)" -l"dcpssacpp" -l"dcpsgapi" -l"ddsuser" -l"ddskernel" -l"ddsserialization" -l"ddsconfparser" -l"ddsconf" -l"ddsdatabase" -l"ddsutil" -l"ddsos" -ldl $(subst lib,-l,$(sort $(basename $(notdir $(wildcard /usr/lib/librt.so /lib/librt.so))))) -lpthread
SRC           = ../src/CheckStatus.cpp ../src/SAL_%s.cpp libSAL%s.cpp
LINK.cc       = $(LD) $(LDFLAGS)
EXPORTFLAGS   = 
endif

#----------------------------------------------------------------------------
#       Local targets
#----------------------------------------------------------------------------

all: $(BIN1)

$(BIN1): $(OBJS1)
	@$(TESTDIRSTART) "$(BTARGETDIR)" $(TESTDIREND) $(MKDIR) "$(BTARGETDIR)"
	$(LINK.cc) $(OBJS21) $(LDLIBS) $(OUTPUT_OPTION)
        
.obj/libSAL%s.o: ../src/libSAL%s.cpp
	@$(TESTDIRSTART) ".obj/../src" $(TESTDIREND) $(MKDIR) ".obj/../src"
	$(COMPILE.cc) $(EXPORTFLAGS) $(OUTPUT_OPTION) ../src/libSAL%s.cpp

## INSERT COMMANDS TEST BIN

$(BIN1): $(OBJS1)
	@$(TESTDIRSTART) "$(BTARGETDIR)" $(TESTDIREND) $(MKDIR) "$(BTARGETDIR)"
	$(LINK.cc) $(OBJS1) $(LDLIBS) $(OUTPUT_OPTION)

$(BIN2): $(OBJS2)
	@$(TESTDIRSTART) "$(BTARGETDIR)" $(TESTDIREND) $(MKDIR) "$(BTARGETDIR)"
	$(LINK.cc) $(OBJS2) $(LDLIBS) $(OUTPUT_OPTION)


.obj/CheckStatus$(OBJEXT): ../src/CheckStatus.cpp
	@$(TESTDIRSTART) ".obj/../src" $(TESTDIREND) $(MKDIR) ".obj/../src"
	$(COMPILE.cc) $(EXPORTFLAGS) $(OUTPUT_OPTION) ../src/CheckStatus.cpp

.obj/SAL_%s$(OBJEXT): ../src/SAL_%s.cpp
	@$(TESTDIRSTART) ".obj/../src" $(TESTDIREND) $(MKDIR) ".obj/../src"
	$(COMPILE.cc) $(EXPORTFLAGS) $(OUTPUT_OPTION) ../src/SAL_%s.cpp

.obj/%sCommander$(OBJEXT): ../src/%sCommander.cpp
	@$(TESTDIRSTART) ".obj/../src" $(TESTDIREND) $(MKDIR) ".obj/../src"
	$(COMPILE.cc) $(EXPORTFLAGS) $(OUTPUT_OPTION) ../src/%sCommander.cpp

.obj/%sController$(OBJEXT): ../src/%sController.cpp
	@$(TESTDIRSTART) ".obj/../src" $(TESTDIREND) $(MKDIR) ".obj/../src"
	$(COMPILE.cc) $(EXPORTFLAGS) $(OUTPUT_OPTION) ../src/%sController.cpp

generated: $(GENERATED_DIRTY)
	@-:

clean:
	-$(RM) $(OBJS)

realclean: clean
	-$(RM) $(BIN)
	-$(RM) .obj/

check-syntax:
	$(COMPILE.cc) $(EXPORTFLAGS) -Wall -Wextra -pedantic -fsyntax-only $(CHK_SOURCES)

#----------------------------------------------------------------------------
#       Dependencies
#----------------------------------------------------------------------------

$(DEPENDENCIES):
	@$(TOUCH) $(DEPENDENCIES)

depend:
	-VDIR=.obj/ $(MPC_ROOT)/depgen.pl  $(CFLAGS) $(CCFLAGS) $(CPPFLAGS) -f $(DEPENDENCIES) $(SRC) 2> $(NUL)

include $(DEPENDENCIES)
"""
    def __init__(self, subsystem, telemetry, events, commands):
        self.subsystem = subsystem
        self.telemetry = telemetry
        self.events = events
        self.commands = commands
    
    def DefineSALObj(self):
        return self.defineSALObj % (self.subsystem, self.subsystem, self.subsystem)
        
    def SALObj(self):
        return self.SALObj % (self.subsystem)
        
    def Code(self):
        i = 1
        forwardDefines = ""
        headerDefines = ""
        cppDefines = ""
        for item in self.telemetry:
            forwardDefines += item.GetForwardDefine() + "\n"
            headerDefines += item.GetHeaderDefine() + "\n"
            cppDefines += item.GetCPPDefine() + "\n"
        for item in self.events:
            forwardDefines += item.GetForwardDefine() + "\n"
            headerDefines += item.GetHeaderDefine() + "\n"
            cppDefines += item.GetCPPDefine() + "\n"
        for item in self.commands:
            forwardDefines += item.GetForwardDefine() + "\n"
            headerDefines += item.GetHeaderDefine() + "\n"
            cppDefines += item.GetCPPDefine() + "\n"
        return self.headerCode % (self.subsystem, self.subsystem, forwardDefines, headerDefines), self.cppCode % (self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, cppDefines), self.makeFile % (self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem, self.subsystem)

        
class SALParameter:
    idlToCpp = { "boolean" : "uint8_t", "octet" : "uint8_t", "short" : "int16_t", "unsigned short" : "uint16_t", "long" : "int32_t", "unsigned long" : "uint32_t", "long long" : "int64_t", "unsigned long long" : "uint64_t", "float" : "float", "double" : "double", "char" : "uint8_t", "string" : "std::string*" }
    idlToLabVIEW = { "boolean" : "U8", "octet" : "U8", "short" : "I16", "unsigned short" : "U16", "long" : "I32", "unsigned long" : "U32", "long long" : "I64", "unsigned long long" : "U64", "float" : "SGL", "double" : "DBL", "char" : "U8", "string" : "String"}
    assign = "        %s.%s = get%s(buffer, &bufferIndex);"
    assignArray = "        for(int32_t i = 0; i < %s; i++) { %s.%s[i] = get%s(buffer, &bufferIndex); }"
    assignString = "        std::string* tmp_%s = getString(buffer, &bufferIndex); %s.%s = *tmp_%s;"
    cleanup = ""
    cleanupString = "        %s.%s = \"\"; delete tmp_%s;"
    set = "        set%s(buffer, &bufferIndex, %s.%s);"
    setString = "        set%s(buffer, &bufferIndex, &%s.%s);"
    setArray = "        for(int32_t i = 0; i < %s; i++) { set%s(buffer, &bufferIndex, %s.%s[i]); }"

    def __init__(self, name, description, type, units, count):
        self.name = name
        self.description = description
        self.type = type
        self.units = units
        self.count = count

    def Assign(self, dataName):
        if self.type != "string":
            if self.count == "1":
                return self.assign % (dataName, self.name, self.idlToLabVIEW[self.type])
            else:
                return self.assignArray % (self.count, dataName, self.name, self.idlToLabVIEW[self.type])
        else:
            return self.assignString % (self.name, dataName, self.name, self.name)
            
    def Cleanup(self, dataName):
        if self.type == "string":
            return self.cleanupString % (dataName, self.name, self.name); 
        return self.cleanup
    
    def Set(self, dataName):
        if self.type != "string":
            if self.count == "1":
                return self.set % (self.idlToLabVIEW[self.type], dataName, self.name)
            else:
                return self.setArray % (self.count, self.idlToLabVIEW[self.type], dataName, self.name)
        else:
            return self.setString % (self.idlToLabVIEW[self.type], dataName, self.name)

class SALCommand:  
    headerForwardDefine = "struct %s_command_%sC;"
    headerCode = "extern int32_t libIssueCommand%s(%s_command_%sC* data);\nextern int32_t libAcceptCommand%s(%s_command_%sC* data);\nextern int32_t libWaitForCompletion%s(int32_t commandId, int32_t timeout);\nextern int32_t libAckCommand%s(int32_t commandId, int32_t ackCode, int32_t errorCode, const char* description);"
    cppCode = "extern \"C\" int32_t libIssueCommand%s(%s_command_%sC* data) { return %sSAL.issueCommand_%s(data); }\nextern \"C\" int32_t libAcceptCommand%s(%s_command_%sC* data) { return %sSAL.acceptCommand_%s(data); }\nextern \"C\" int32_t libWaitForCompletion%s(int32_t commandId, int32_t timeout) { return %sSAL.waitForCompletion_%s(commandId, timeout); }\nextern \"C\" int32_t libAckCommand%s(int32_t commandId, int32_t ackCode, int32_t errorCode, const char* description) { return %sSAL.ackCommand_%s(commandId, ackCode, errorCode, (char*)description); }"

    def __init__(self, subsystem, version, author, name, parameters):
        self.subsystem = subsystem
        self.version = version
        self.author = author
        self.name = name
        self.parameters = parameters
        
    def GetForwardDefine(self):
        return self.headerForwardDefine % (self.subsystem, self.name)
        
    def GetHeaderDefine(self):
        return self.headerCode % (self.name, self.subsystem, self.name, self.name, self.subsystem, self.name, self.name, self.name)
        
    def GetCPPDefine(self):
        return self.cppCode % (self.name, self.subsystem, self.name, self.subsystem, self.name, self.name, self.subsystem, self.name, self.subsystem, self.name, self.name, self.subsystem, self.name, self.name, self.subsystem, self.name)

class SALEvent:
    headerForwardDefine = "struct %s_logevent_%sC;"
    headerCode = "extern int32_t libLogEvent%s(%s_logevent_%sC* data);\nextern int32_t libGetEvent%s(%s_logevent_%sC* data);"
    cppCode = "extern \"C\" int32_t libLogEvent%s(%s_logevent_%sC* data) { return %sSAL.logEvent_%s(data, 0); }\nextern \"C\" int32_t libGetEvent%s(%s_logevent_%sC* data) { return %sSAL.getEvent_%s(data); }"
        
    def __init__(self, subsystem, version, author, name, parameters):
        self.subsystem = subsystem
        self.version = version
        self.author = author
        self.name = name
        self.parameters = parameters
        
    def GetForwardDefine(self):
        return self.headerForwardDefine % (self.subsystem, self.name)
        
    def GetHeaderDefine(self):
        return self.headerCode % (self.name, self.subsystem, self.name, self.name, self.subsystem, self.name)
        
    def GetCPPDefine(self):
        return self.cppCode % (self.name, self.subsystem, self.name, self.subsystem, self.name, self.name, self.subsystem, self.name, self.subsystem, self.name)
    

class SALTelemetry:
    headerForwardDefine = "struct %s_%sC;"
    headerCode = "extern int32_t libPutSample%s(%s_%sC* data);\nextern int32_t libGetSample%s(%s_%sC* data);\nextern int32_t libGetNextSample%s(%s_%sC* data);"
    cppCode = "extern \"C\" int32_t libPutSample%s(%s_%sC* data) { return %sSAL.putSample_%s(data); }\nextern \"C\" int32_t libGetSample%s(%s_%sC* data) { return %sSAL.getSample_%s(data); }\nextern \"C\" int32_t libGetNextSample%s(%s_%sC* data) { return %sSAL.getNextSample_%s(data); }"

    def __init__(self, subsystem, version, author, name, parameters):
        self.subsystem = subsystem
        self.version = version
        self.author = author
        self.name = name
        self.parameters = parameters
        
    def GetForwardDefine(self):
        return self.headerForwardDefine % (self.subsystem, self.name)
        
    def GetHeaderDefine(self):
        return self.headerCode % (self.name, self.subsystem, self.name, self.name, self.subsystem, self.name, self.name, self.subsystem, self.name)
        
    def GetCPPDefine(self):
        return self.cppCode % (self.name, self.subsystem, self.name, self.subsystem, self.name, self.name, self.subsystem, self.name, self.subsystem, self.name, self.name, self.subsystem, self.name, self.subsystem, self.name)

if len(sys.argv) != 4:
    print """
Version: 1.0
    
usage: *.py <SubSystem> <UMLFile> <OutputDirectory>
example: *.py m2ms D:\Temp\SALTemp.xml D:\Temp

Notes:
    1. Commands must be a direct child of a package named Command
    2. Commands must not be named Command, otherwise it will be ignored
    3. Events must be a direct child of a package named Event
    4. Events must not be named Event, otherwise it will be ignored
    5. Telemetry must be a direct child of a package named Telemetry
    6. Telemetry must not be named Telemetry, otherwise it will be ignored
    7. If you want parameters to have units defined, create a new tag named 'unit'"""
else:
    uml = UMLParser()
    uml.Open(sys.argv[1], sys.argv[2])
    uml.Parse(sys.argv[3])