include ../Makefile.inc

all: dcrawdump

CPPFLAGS := -I../src/NiFpga

dcrawdump: dcrawdump.cpp.o
	@echo '[LD ] $@'
	${co}$(CPP) -o $@ $^

%.cpp.o: %.cpp.d
	@echo '[CPP] $(patsubst %.d,%,$<)'
	${co}$(CPP) ${CPPFLAGS} -c -o $@ $(patsubst %.d,%,$<)

%.cpp.d: %.cpp
	@echo '[DPP] $<'
	${co}$(CPP) ${CPPFLAGS} -M $< -MF $@ -MT '$(patsubst %.cpp,%.o,$<) $@'
