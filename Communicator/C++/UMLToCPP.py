
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
        with open("%s\\SALPipe.cpp" % (self.outputDirectory), "w") as output:
            output.write(file.Code())
        
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
    code = """
#include <sys/socket.h>       /*  socket definitions        */
#include <sys/types.h>        /*  socket types              */
#include <arpa/inet.h>        /*  inet (3) funtions         */
#include <unistd.h>           /*  misc. UNIX functions      */

#include <stdlib.h>
#include <stdio.h>
#include <cstring>

#include <string>
#include <sstream>
#include <iostream>
#include "SAL_m1m3.h"
#include "ccpp_sal_m1m3.h"
#include "os.h"

#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>

using namespace DDS;
using namespace m1m3;

/*  Global constants  */

#define ECHO_PORT          (2002)
#define MAX_LINE           (1000)
#define LISTENQ        (1024)   /*  Backlog for listen()   */

typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
typedef unsigned char bool_t;

#define MESSAGE_TYPE_NONE 0
#define MESSAGE_TYPE_PUT_SAMPLE 1
#define MESSAGE_TYPE_GET_NEXT_SAMPLE 2
#define MESSAGE_TYPE_LOG_EVENT 3
#define MESSAGE_TYPE_GET_EVENT 4
#define MESSAGE_TYPE_ISSUE_COMMAND 5
#define MESSAGE_TYPE_COMMAND_ID 6
#define MESSAGE_TYPE_WAIT_FOR_COMPLETION 7
#define MESSAGE_TYPE_ACCEPT_COMMAND 8
#define MESSAGE_TYPE_ACK_COMMAND 9
#define MESSAGE_TYPE_SHUTDOWN 255

#define MESSAGE_TOPIC_NONE 0
%s
#define TOPIC_COUNT 255

#define POLL_ERROR -1
#define POLL_TIMEOUT 0

#define HEADER_BUFFER_SIZE 6
#define DATA_BUFFER_SIZE 1048576

#pragma pack(push, 1)
typedef struct MessageHeader {
	uint8_t type;
	uint8_t topic;
	uint32_t size;
};
#pragma pack(pop)

bool_t enabledTopics[TOPIC_COUNT];
os_time delay_10ms = { 0, 10000000 };
char *endptr; 
int16_t port;
bool_t stayAlive = false;
int32_t listenerHandle;
sockaddr_in listenerAddress;
bool_t keepRunning = true;
int connectionHandle;
pollfd pollHandle;
int32_t rc;
MessageHeader messageHeader;
uint8_t headerBuffer[HEADER_BUFFER_SIZE];
int32_t headerIndex;
uint8_t buffer[DATA_BUFFER_SIZE];
int32_t bufferIndex;
int32_t commandId;
%s
%s

int8_t getI8(uint8_t *b, int32_t *i) {
    return b[(*i)++];
}

int16_t getI16(uint8_t *b, int32_t *i) {
    return (((int16_t)(b[(*i)++])) << 8) + ((int16_t)(b[(*i)++]));
}

int32_t getI32(uint8_t *b, int32_t *i) {
    return (((int32_t)(b[(*i)++])) << 24) + (((int32_t)(b[(*i)++])) << 16) + (((int32_t)(b[(*i)++])) << 8) + ((int32_t)(b[(*i)++]));
}

int64_t getI64(uint8_t *b, int32_t *i) {
    return (((int64_t)(b[(*i)++])) << 56) + (((int64_t)(b[(*i)++])) << 48) + (((int64_t)(b[(*i)++])) << 40) + (((int64_t)(b[(*i)++])) << 32) + (((int64_t)(b[(*i)++])) << 24) + (((int64_t)(b[(*i)++])) << 16) + (((int64_t)(b[(*i)++])) << 8) + ((int64_t)(b[(*i)++]));
}

uint8_t getU8(uint8_t *b, int32_t *i) {
    return b[(*i)++];
}

uint16_t getU16(uint8_t *b, int32_t *i) {
    return (((uint16_t)(b[(*i)++])) << 8) + ((uint16_t)(b[(*i)++]));
}

uint32_t getU32(uint8_t *b, int32_t *i) {
    return (((uint32_t)(b[(*i)++])) << 24) + (((uint32_t)(b[(*i)++])) << 16) + (((uint32_t)(b[(*i)++])) << 8) + ((uint32_t)(b[(*i)++]));
}

uint64_t getU64(uint8_t *b, int32_t *i) {
    return (((uint64_t)(b[(*i)++])) << 56) + (((uint64_t)(b[(*i)++])) << 48) + (((uint64_t)(b[(*i)++])) << 40) + (((uint64_t)(b[(*i)++])) << 32) + (((uint64_t)(b[(*i)++])) << 24) + (((uint64_t)(b[(*i)++])) << 16) + (((uint64_t)(b[(*i)++])) << 8) + ((uint64_t)(b[(*i)++]));
}

float getSGL(uint8_t *b, int32_t *i) {
    float data;
    memcpy(&data, (b + (*i)), 4);
    (*i) += 4;
    return data;
}

double getDBL(uint8_t *b, int32_t *i) {
    double data;
    memcpy(&data, (b + (*i)), 8);
    (*i) += 8;
    return data;
}

std::string* getString(uint8_t *b, int32_t *i) {
    int32_t length = getI32(b, i);
    char *temp = new char[length + 1];
    for(int32_t j = 0; j < length; j++) {
        temp[j] = b[(*i)++];
    }
    temp[length] = 0;
    std::string *str = new std::string((char*)temp);
    delete[] temp;
    return str;
}

void setI8(uint8_t *b, int32_t *i, int8_t data) {
    b[(*i)++] = data;
}

void setI16(uint8_t *b, int32_t *i, int16_t data) {
    b[(*i)++] = (data >> 8) & 0xFF;
    b[(*i)++] = data & 0xFF;
}

void setI32(uint8_t *b, int32_t *i, int32_t data) {
    b[(*i)++] = (data >> 24) & 0xFF;
    b[(*i)++] = (data >> 16) & 0xFF;
    b[(*i)++] = (data >> 8) & 0xFF;
    b[(*i)++] = data & 0xFF;
}

void setI64(uint8_t *b, int32_t *i, int64_t data) {
    b[(*i)++] = (data >> 56) & 0xFF;
    b[(*i)++] = (data >> 48) & 0xFF;
    b[(*i)++] = (data >> 40) & 0xFF;
    b[(*i)++] = (data >> 32) & 0xFF;
    b[(*i)++] = (data >> 24) & 0xFF;
    b[(*i)++] = (data >> 16) & 0xFF;
    b[(*i)++] = (data >> 8) & 0xFF;
    b[(*i)++] = data & 0xFF;
}

void setU8(uint8_t *b, int32_t *i, uint8_t data) {
    b[(*i)++] = data;
}

void setU16(uint8_t *b, int32_t *i, uint16_t data) {
    b[(*i)++] = (data >> 8) & 0xFF;
    b[(*i)++] = data & 0xFF;
}

void setU32(uint8_t *b, int32_t *i, uint32_t data) {
    b[(*i)++] = (data >> 24) & 0xFF;
    b[(*i)++] = (data >> 16) & 0xFF;
    b[(*i)++] = (data >> 8) & 0xFF;
    b[(*i)++] = data & 0xFF;
}

void setU64(uint8_t *b, int32_t *i, uint64_t data) {
    b[(*i)++] = (data >> 56) & 0xFF;
    b[(*i)++] = (data >> 48) & 0xFF;
    b[(*i)++] = (data >> 40) & 0xFF;
    b[(*i)++] = (data >> 32) & 0xFF;
    b[(*i)++] = (data >> 24) & 0xFF;
    b[(*i)++] = (data >> 16) & 0xFF;
    b[(*i)++] = (data >> 8) & 0xFF;
    b[(*i)++] = data & 0xFF;
}

void setSGL(uint8_t *b, int32_t *i, float data) {
    memcpy((b + (*i)), &data, 4);
    (*i) += 4;
}

void setDBL(uint8_t *b, int32_t *i, double data) {
    memcpy((b + (*i)), &data, 8);
    (*i) += 8;
}

void setString(uint8_t *b, int32_t *i, std::string* data) {
    setI32(b, i, data->length());
    const char *str = data->c_str();
    for(uint32_t j = 0; j < data->length(); j++) {
        b[(*i)++] = str[j];
    }
}

bool_t readClientHeader() {
    uint8_t *bufferCopy = buffer;
    int32_t remaining = 6;
    if (read(connectionHandle, bufferCopy, 1) == 1) {
        bufferCopy += 1;
        remaining -= 1;
        while(remaining > 0) {
            int32_t got = read(connectionHandle, bufferCopy, remaining);
            bufferCopy += got;
            remaining -= got;
        }
        bufferIndex = 0;
        messageHeader.type = getU8(buffer, &bufferIndex);
        messageHeader.topic = getU8(buffer, &bufferIndex);
        messageHeader.size = getI32(buffer, &bufferIndex);
        printf("GOT %%d %%d %%d\\n", messageHeader.type, messageHeader.topic, messageHeader.size);
        return true;
    }
    return false;
}

bool_t readClientData() {
    uint8_t *bufferCopy = buffer;
    int32_t remaining = messageHeader.size;
    while(remaining > 0) {
        int32_t got = read(connectionHandle, bufferCopy, remaining);
        bufferCopy += got;
        remaining -= got;
    }
    return true;
}

bool_t writeBuffer(uint8_t *buffer, int32_t count) {
    int32_t written = 0;
    while (count > 0) {
        written = write(connectionHandle, buffer, count);
        if (written <= 0) {
            if (errno == EINTR) {
                written = 0;
            }
            else {
                return false;
            }
        }
        count -= written;
        buffer += written;
    }
    return true;
}

bool_t writeClientHeader(uint8_t type, uint8_t topic, int32_t size) {
    printf("Writing header {%%d, %%d, %%d}\\n", type, topic, size);
    headerIndex = 0;
    setU8(headerBuffer, &headerIndex, type);
    setU8(headerBuffer, &headerIndex, topic);
    setI32(headerBuffer, &headerIndex, size);
    return writeBuffer(headerBuffer, sizeof(MessageHeader));
}

%s

void processPutSampleClientRequest() {
    printf("PUT_SAMPLE ");
    switch(messageHeader.topic) {
%s
        default:
            printf("Unknown message header topic %%d\\n", messageHeader.topic);
            break;
    }
}

void processGetNextSampleClientRequest() {
    printf("GET_NEXT_SAMPLE ");
    switch(messageHeader.topic) {
        default:
            printf("Unknown message header topic %%d\\n", messageHeader.topic);
            break;
    }
}

%s

void processLogEventClientRequest() {
    printf("LOG_EVENT ");
    switch(messageHeader.topic) {
%s
        default:
            printf("Unknown message header topic %%d\\n", messageHeader.topic);
            break;
    }
}

void processGetEventClientRequest() {
    printf("GET_EVENT ");
    switch(messageHeader.topic) {
        default:
            printf("Unknown message header topic %%d\\n", messageHeader.topic);
            break;
    }    
}

void sendCommandId(uint8_t topic) {
    printf("COMMAND_ID %%d\\n", commandId);
    bufferIndex = 0;
    setI32(buffer, &bufferIndex, commandId);
    writeClientHeader(MESSAGE_TYPE_COMMAND_ID, topic, bufferIndex);
    writeBuffer(buffer, bufferIndex);
}

%s

void processIssueCommandClientRequest() {
    printf("ISSUE_COMMAND ");
    switch(messageHeader.topic) {
%s
        default:
            printf("Unknown message header topic %%d\\n", messageHeader.topic);
            break;
    }
}

void processCommandIdClientRequest() {
    printf("COMMAND_ID ");
    switch(messageHeader.topic) {
        default:
            printf("Unknown message header topic %%d\\n", messageHeader.topic);
            break;
    }
}

void processWaitForCompletionClientRequest() {
    printf("WAIT_FOR_COMPLETION ");
    switch(messageHeader.topic) {
        default:
            printf("Unknown message header topic %%d\\n", messageHeader.topic);
            break;
    }
}

void processAcceptCommandClientRequest() {
    printf("ACCEPT_COMMAND ");
    switch(messageHeader.topic) {
        default:
            printf("Unknown message header topic %%d\\n", messageHeader.topic);
            break;
    }
}

void processAckCommandClientRequest() {
    printf("ACK_COMMAND ");
    switch(messageHeader.topic) {
        default:
            printf("Uknown message header topic %%d\\n", messageHeader.topic);
            break;
    }
}

void processShutdownClientRequest() {
    printf("Shutting down SALPipe\\n");
    keepRunning = false;
}

void processClientRequest() {
    bool_t keepProcessingRequests = true;
    while(keepProcessingRequests && keepRunning) {
        rc = poll(&pollHandle, 1, 1);
        switch(rc) {
            case POLL_ERROR:
                keepProcessingRequests = false;
                break;
            case POLL_TIMEOUT:
                keepProcessingRequests = false;
                break;
            default:
                if (readClientHeader()) {
                    if (readClientData()) {
                        switch(messageHeader.type) {
                            case MESSAGE_TYPE_PUT_SAMPLE: processPutSampleClientRequest(); break;
                            case MESSAGE_TYPE_GET_NEXT_SAMPLE: processGetNextSampleClientRequest(); break;
                            case MESSAGE_TYPE_LOG_EVENT: processLogEventClientRequest(); break;
                            case MESSAGE_TYPE_GET_EVENT: processGetEventClientRequest(); break;
                            case MESSAGE_TYPE_ISSUE_COMMAND: processIssueCommandClientRequest(); break;
                            case MESSAGE_TYPE_COMMAND_ID: processCommandIdClientRequest(); break;
                            case MESSAGE_TYPE_WAIT_FOR_COMPLETION: processWaitForCompletionClientRequest(); break;
                            case MESSAGE_TYPE_ACCEPT_COMMAND: processAcceptCommandClientRequest(); break;
                            case MESSAGE_TYPE_ACK_COMMAND: processAckCommandClientRequest(); break;
                            case MESSAGE_TYPE_SHUTDOWN: processShutdownClientRequest(); break;
                            default:
                                printf("Unknown message header type %%d\\n", messageHeader.type);
                                break;
                        }
                    }
                }
                break;
        }
    }
}

%s

void processGetNextSampleSALRequest() {
%s
}

%s

void processGetEventSALRequest() {
%s
}

%s

void processAcceptCommandSALRequest() {
%s
}

void processSALRequest() {
    processGetNextSampleSALRequest();
    processGetEventSALRequest();
    processAcceptCommandSALRequest();
}

int main(int argumentCount, char *argumentValues[]) {
    memset(&listenerAddress, 0, sizeof(sockaddr_in));
	memset(&messageHeader, 0, sizeof(MessageHeader));

    // Process arguments for port number
    if ( argumentCount >= 4 ) {
        port = strtol(argumentValues[1], &endptr, 0);
        if ( *endptr ) {
            fprintf(stderr, "SALPIPE: Invalid port number.\\n");
            exit(EXIT_FAILURE);
        }
        
        stayAlive = strtol(argumentValues[2], &endptr, 0);
        if (*endptr) {
            fprintf(stderr, "SALPIPE: Invalid stay alive.\\n");
            exit(EXIT_FAILURE);
        }
        
        int enabledTopic = 0;
        for(int i = 3; i < argumentCount; i++) {
            enabledTopic = strtol(argumentValues[i], &endptr, 0);
            if (*endptr) {
                fprintf(stderr, "Invalid enabled topic.\\n");
                exit(EXIT_FAILURE);
            }
            if (enabledTopic != -1) {
                enabledTopics[enabledTopic] = true;
            }
            else {
                for(int j = 0; j < TOPIC_COUNT; j++) {
                    enabledTopics[j] = true;
                }
            }
        }
    }
    else if ( argumentCount < 2 ) {
        port = ECHO_PORT;
    }
    else {
        fprintf(stderr, "SALPIPE: Invalid arguments.\\n");
        exit(EXIT_FAILURE);
    }

    // Create listener handle
    if ((listenerHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fprintf(stderr, "SALPIPE: Error creating listening socket.\\n");
        exit(EXIT_FAILURE);
    }

    // Create listener address
    listenerAddress.sin_family      = AF_INET;
    listenerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    listenerAddress.sin_port        = htons(port);

    // Bind listener
    if ( bind(listenerHandle, (struct sockaddr *) &listenerAddress, sizeof(listenerAddress)) < 0 ) {
        fprintf(stderr, "SALPIPE: Error calling bind()\\n");
        exit(EXIT_FAILURE);
    }
    
    // Set socket reuse option
    int one = 1;
    if (setsockopt(listenerHandle, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) < 0) {
        fprintf(stderr, "SALPIPE: setsockopt(SO_REUSEADDR) failed");
        exit(EXIT_FAILURE);
    }
    
    // Start listener
    if ( listen(listenerHandle, LISTENQ) < 0 ) {
        fprintf(stderr, "SALPIPE: Error calling listen()\\n");
        exit(EXIT_FAILURE);
    }
    
    // Wait for a client to connect
    printf("Waiting for client\\n");
    if ((connectionHandle = accept(listenerHandle, NULL, NULL) ) < 0 ) {
        fprintf(stderr, "SALPIPE: Error calling accept()\\n");
        exit(EXIT_FAILURE);
    }

    // Initialization Start
    printf("Initializing ");
    
    // Initialize Telemetry
    printf("Telemetry ");
%s
    
    // Initialize Events
    printf("Events ");
%s
    
    // Initialize Commands
    printf("Commands ");
%s

    do {
        // Initialization Done
        printf("Done\\n");
        keepRunning = true;
        headerBuffer[0] = 0xFF;
        writeBuffer(headerBuffer, 1);
        
        // Setup polling
        memset(&pollHandle, 0, sizeof(pollfd));
        pollHandle.fd = connectionHandle;
        pollHandle.events = POLLIN;
        
        int flag = 1;
        rc = setsockopt(connectionHandle, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
        if (rc < 0) {
            printf("SALPIPE: Problem with TCP_NODELAY\\n");
        }

        while (keepRunning) {
            // Execute service loop
            sleep(0);
            
            // Check SAL clients
            processSALRequest();
            
            // Read data from client
            processClientRequest();
        }
        
        // Close client
        printf("Closing client connection\\n");
        if (close(connectionHandle) < 0) {
            fprintf(stderr, "SALPIPE: Error calling close()\\n");
            exit(EXIT_FAILURE);
        }
        
        if (stayAlive) {
            // Wait for a client to connect
            printf("Waiting for client\\n");
            if ((connectionHandle = accept(listenerHandle, NULL, NULL) ) < 0 ) {
                fprintf(stderr, "SALPIPE: Error calling accept()\\n");
                exit(EXIT_FAILURE);
            }
        }
    } while(stayAlive);

    // Close SAL
    printf("Shutting down SAL\\n");
    m1m3SAL.salShutdown();
}
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
        defineTopics = ""
        defineTopicDatas = ""
        defineTelemetryClientRequests = ""
        telemetryClientRequests = ""
        defineEventClientRequests = ""
        eventClientRequests = ""
        defineCommandClientRequests = ""
        commandClientRequests = ""
        defineTelemetrySALRequests = ""
        telemetrySALRequests = ""
        defineEventSALRequests = ""
        eventSALRequests = ""
        defineCommandSALRequests = ""
        commandSALRequests = ""
        telemetryInits = ""
        eventInits = ""
        commandInits = ""
        for item in self.telemetry:
            defineTopics += item.DefineTopic(i) + "\n"
            defineTopicDatas += item.DefineTopicData() + "\n"
            defineTelemetryClientRequests += item.DefineClientRequest() + "\n"
            telemetryClientRequests += item.ClientRequest() + "\n"
            defineTelemetrySALRequests += item.DefineSALRequest() + "\n"
            telemetrySALRequests += item.SALRequest() + "\n"
            telemetryInits += item.InitTopic() + "\n"
            i += 1
        for item in self.events:
            defineTopics += item.DefineTopic(i) + "\n"
            defineTopicDatas += item.DefineTopicData() + "\n"
            defineEventClientRequests += item.DefineClientRequest() + "\n"
            eventClientRequests += item.ClientRequest() + "\n"
            defineEventSALRequests += item.DefineSALRequest() + "\n"
            eventSALRequests += item.SALRequest() + "\n"
            eventInits += item.InitTopic() + "\n"
            i += 1
        for item in self.commands:
            defineTopics += item.DefineTopic(i) + "\n"
            defineTopicDatas += item.DefineTopicData() + "\n"
            defineCommandClientRequests += item.DefineClientRequest() + "\n"
            commandClientRequests += item.ClientRequest() + "\n"
            defineCommandSALRequests += item.DefineSALRequest() + "\n"
            commandSALRequests += item.SALRequest() + "\n"
            commandInits += item.InitTopic() + "\n"
            i += 1
        
        return self.code % (defineTopics, self.DefineSALObj(), defineTopicDatas, defineTelemetryClientRequests, telemetryClientRequests, defineEventClientRequests, eventClientRequests, defineCommandClientRequests, commandClientRequests, defineTelemetrySALRequests, telemetrySALRequests, defineEventSALRequests, eventSALRequests, defineCommandSALRequests, commandSALRequests, telemetryInits, eventInits, commandInits)

        
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
    defineTopic = "#define MESSAGE_TOPIC_%s %d"
    defineTopicData = "m1m3_command_%sC command%sData;"
    defineProcessIssueCommandClientRequest = """
void processIssueCommand%sClientRequest() {
    printf("%s\\n");
    if (enabledTopics[MESSAGE_TOPIC_%s]) {
        bufferIndex = 0;
%s
        commandId = %sSAL.issueCommand_%s(&%s);
%s
        sendCommandId(MESSAGE_TOPIC_%s);
    }
}
"""
    processIssueCommandClientRequest = "        case MESSAGE_TOPIC_%s: processIssueCommand%sClientRequest(); break;"
    defineProcessAcceptCommandSALRequest = """
void processAcceptCommand%sSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_%s]) {
        commandId = %sSAL.acceptCommand_%s(&%s);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND %s\\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
%s
            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_%s, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}"""
    processAcceptCommandSALRequest = "    processAcceptCommand%sSALRequest();"
    initTopic = "    if (enabledTopics[MESSAGE_TOPIC_%s]) { %sSAL.salCommand(\"m1m3_command_%s\"); %sSAL.salProcessor(\"m1m3_command_%s\"); }"
    commandDataTemplate = "command%sData"

    def __init__(self, subsystem, version, author, name, parameters):
        self.subsystem = subsystem
        self.version = version
        self.author = author
        self.name = name
        self.parameters = parameters
        
    def GetCommandData(self):
        return self.commandDataTemplate % (self.name)
        
    def DefineTopic(self, number):
        return self.defineTopic % (self.name, number)
        
    def DefineTopicData(self):
        return self.defineTopicData % (self.name, self.name)

    def DefineClientRequest(self):
        assign = ""
        cleanup = ""
        for param in self.parameters:
            assign += param.Assign(self.GetCommandData()) + "\n"
            cleanup += param.Cleanup(self.GetCommandData()) + "\n"
        return self.defineProcessIssueCommandClientRequest % (self.name, self.name, self.name, assign, self.subsystem, self.name, self.GetCommandData(), cleanup, self.name)
        
    def ClientRequest(self):
        return self.processIssueCommandClientRequest % (self.name, self.name)
        
    def DefineSALRequest(self):
        setVars = ""
        for param in self.parameters:
            setVars += param.Set(self.GetCommandData()) + "\n"
        return self.defineProcessAcceptCommandSALRequest % (self.name, self.name, self.subsystem, self.name, self.GetCommandData(), self.name, setVars, self.name)
            
    def SALRequest(self):
        return self.processAcceptCommandSALRequest % (self.name)
        
    def InitTopic(self):
        return self.initTopic % (self.name, self.subsystem, self.name, self.subsystem, self.name)
        

class SALEvent:
    defineTopic = "#define MESSAGE_TOPIC_%s %d"
    defineTopicData = "m1m3_logevent_%sC event%sData;"
    defineProcessLogEventClientRequest = """
void processLogEvent%sClientRequest() {
    printf("%s\\n");
    if (enabledTopics[MESSAGE_TOPIC_%s]) {
        bufferIndex = 0;
%s
        %s.priority = getI32(buffer, &bufferIndex);
        %sSAL.logEvent_%s(&%s, %s.priority);
%s
    }
}
"""
    processLogEventClientRequest = "        case MESSAGE_TOPIC_%s: processLogEvent%sClientRequest(); break;"
    defineProcessGetEventSALRequest = """
void processGetEvent%sSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_%s]) {
        rc = %sSAL.getEvent_%s(&%s);
        if (rc == SAL__OK) {
            printf("GET_EVENT %s\\n");
            bufferIndex = 0;
%s
            setI32(buffer, &bufferIndex, %s.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_%s, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}"""
    procesGetEventSALRequest = "        processGetEvent%sSALRequest();"
    initTopic = "    if (enabledTopics[MESSAGE_TOPIC_%s]) { %sSAL.salEvent(\"m1m3_logevent_%s\"); }"
    eventDataTemplate = "event%sData"
    
    def __init__(self, subsystem, version, author, name, parameters):
        self.subsystem = subsystem
        self.version = version
        self.author = author
        self.name = name
        self.parameters = parameters
        
    def GetEventData(self):
        return self.eventDataTemplate % (self.name)

    def DefineTopic(self, number):
        return self.defineTopic % (self.name, number)
        
    def DefineTopicData(self):
        return self.defineTopicData % (self.name, self.name)
        
    def DefineClientRequest(self):
        assign = ""
        cleanup = ""
        for param in self.parameters:
            assign += param.Assign(self.GetEventData()) + "\n"
            cleanup += param.Cleanup(self.GetEventData()) + "\n"
        return self.defineProcessLogEventClientRequest % (self.name, self.name, self.name, assign, self.GetEventData(), self.subsystem, self.name, self.GetEventData(), self.GetEventData(), cleanup)
        
    def ClientRequest(self):
        return self.processLogEventClientRequest % (self.name, self.name)
        
    def DefineSALRequest(self):
        setVars = ""
        for param in self.parameters:
            setVars += param.Set(self.GetEventData()) + "\n"
        return self.defineProcessGetEventSALRequest % (self.name, self.name, self.subsystem, self.name, self.GetEventData(), self.name, setVars, self.GetEventData(), self.name)
        
    def SALRequest(self):
        return self.procesGetEventSALRequest % (self.name)
        
    def InitTopic(self):
        return self.initTopic % (self.name, self.subsystem, self.name)
    

class SALTelemetry:
    defineTopic = "#define MESSAGE_TOPIC_%s %d"
    defineTopicData = "m1m3_%sC telemetry%sData;"
    defineProcessPutSampleClientRequest = """
void processPutSample%sClientRequest() {
    printf("%s\\n");
    if (enabledTopics[MESSAGE_TOPIC_%s]) {
        bufferIndex = 0;
%s
        %sSAL.putSample_%s(&%s);
%s
    }
}
"""
    processPutSampleClientRequest = "        case MESSAGE_TOPIC_%s: processPutSample%sClientRequest(); break;"
    defineProcessGetNextSampleSALRequest = """
void processGetNextSample%sSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_%s]) {
        rc = %sSAL.getNextSample_%s(&%s);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE %s\\n");
            bufferIndex = 0;
%s
            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_%s, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}"""
    procesGetNextSampleSALRequest = "        processGetNextSample%sSALRequest();"
    initTopic = "    if (enabledTopics[MESSAGE_TOPIC_%s]) { %sSAL.salTelemetryPub(\"%s_%s\"); %sSAL.salTelemetrySub(\"%s_%s\"); }"
    telemetryDataTemplate = "telemetry%sData"

    def __init__(self, subsystem, version, author, name, parameters):
        self.subsystem = subsystem
        self.version = version
        self.author = author
        self.name = name
        self.parameters = parameters

    def GetTelemetryData(self):
        return self.telemetryDataTemplate % (self.name)

    def DefineTopic(self, number):
        return self.defineTopic % (self.name, number)
        
    def DefineTopicData(self):
        return self.defineTopicData % (self.name, self.name)
        
    def DefineClientRequest(self):
        assign = ""
        cleanup = ""
        for param in self.parameters:
            assign += param.Assign(self.GetTelemetryData()) + "\n"
            cleanup += param.Cleanup(self.GetTelemetryData()) + "\n"
        return self.defineProcessPutSampleClientRequest % (self.name, self.name, self.name, assign, self.subsystem, self.name, self.GetTelemetryData(), cleanup)
        
    def ClientRequest(self):
        return self.processPutSampleClientRequest % (self.name, self.name)
        
    def DefineSALRequest(self):
        setVars = ""
        for param in self.parameters:
            setVars += param.Set(self.GetTelemetryData()) + "\n"
        return self.defineProcessGetNextSampleSALRequest % (self.name, self.name, self.subsystem, self.name, self.GetTelemetryData(), self.name, setVars, self.name)
        
    def SALRequest(self):
        return self.procesGetNextSampleSALRequest % (self.name)
   
    def InitTopic(self):
        return self.initTopic % (self.name, self.subsystem, self.subsystem, self.name, self.subsystem, self.subsystem, self.name)

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