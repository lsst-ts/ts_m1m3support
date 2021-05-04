include Makefile.inc

.PHONY: all clean deploy tests FORCE doc simulator

# Add inputs and outputs from these tool invocations to the build variables 
#

# All Target
all: ts-M1M3support $(m1m3sscli)

src/libM1M3SS.a: FORCE
	$(MAKE) -C src libM1M3SS.a

# Tool invocations
ts-M1M3support: src/ts-M1M3support.cpp.o src/libM1M3SS.a
	@echo '[LD ] $@'
	${co}$(CPP) $(LIBS_FLAGS) -o $@ $^ $(LIBS) $(CRIOCPP)/lib/libcRIOcpp.a

m1m3sscli: src/m1m3sscli.cpp.o src/libM1M3SS.a $(CRIOCPP)/lib/libcRIOcpp.a
	@echo '[LD ] $@'
	${co}$(CPP) -o $@ $^ -L/usr/lib64 -lpthread -lreadline -ldl $(CRIOCPP)/lib/libcRIOcpp.a

# Other Targets
clean:
	@$(foreach file,ts_M1M3Support src/ts_M1M3Support.cpp.o src/m1m3cli.cpp.o doc, echo '[RM ] ${file}'; $(RM) -r $(file);)
	@$(foreach dir,src tests,$(MAKE) -C ${dir} $@;)

# file targets
src/%.cpp.o: src/%.cpp
	$(MAKE) -C src $(patsubst src/%,%,$@)

CRIO_IP = 10.0.0.11

deploy: ts-M1M3support m1m3cli
	@echo '[SCP] $^'
	${co}scp $^ admin@${CRIO_IP}:
	@echo '[SCP] Bitfiles/NiFpga_M1M3SupportFPGA.lvbitx'
	${co}scp Bitfiles/NiFpga_M1M3SupportFPGA.lvbitx admin@${CRIO_IP}:Bitfiles

tests: tests/Makefile tests/*.cpp
	@${MAKE} -C tests

run_tests: tests
	@${MAKE} -C tests run

junit: tests
	@${MAKE} -C tests junit

doc:
	${co}doxygen Doxyfile

simulator:
	@${MAKE} SIMULATOR=1
