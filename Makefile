include Makefile.inc

.PHONY: all clean deploy tests FORCE doc simulator ipk

# Add inputs and outputs from these tool invocations to the build variables 
#

# All Target
all: ts-M1M3supportd m1m3sscli

src/libM1M3SS.a: FORCE
	$(MAKE) -C src libM1M3SS.a

# Tool invocations
ts-M1M3supportd: src/ts-M1M3supportd.cpp.o src/libM1M3SS.a
	@echo '[LD ] $@'
	${co}$(CPP) $(LIBS_FLAGS) -o $@ $^ $(CRIOCPP)/lib/libcRIOcpp.a $(LIBS)

m1m3sscli: src/m1m3sscli.cpp.o src/libM1M3SS.a $(CRIOCPP)/lib/libcRIOcpp.a
	@echo '[LD ] $@'
	${co}$(CPP) $(LIBS_FLAGS) -o $@ $^ $(CRIOCPP)/lib/libcRIOcpp.a $(LIBS) $(shell pkg-config --libs readline) -lreadline

# Other Targets
clean:
	@$(foreach file,ts-M1M3Supportd *.ipk ipk, echo '[RM ] ${file}'; $(RM) -r $(file);)
	@$(foreach dir,src tests,$(MAKE) -C ${dir} $@;)

# file targets
src/%.cpp.o: src/%.cpp
	$(MAKE) -C src $(patsubst src/%,%,$@)

CRIO_IP = 10.0.0.11

deploy: ts-M1M3supportd m1m3cli
	@echo '[SCP] $^'
	${co}scp $^ admin@${CRIO_IP}:
	@echo '[SCP] Bitfiles/NiFpga_M1M3SupportFPGA.lvbitx'
	${co}scp Bitfiles/NiFpga_M1M3SupportFPGA.lvbitx admin@${CRIO_IP}:Bitfiles

tests: tests/Makefile tests/*.cpp
	@${MAKE} SIMULATOR=1 -C tests

run_tests: tests
	@${MAKE} -C tests run

junit: tests
	@${MAKE} SIMULATOR=1 -C tests junit

doc:
	${co}doxygen Doxyfile

simulator:
	${co}${MAKE} SIMULATOR=1 DEBUG=1 -C .
	@${MAKE} SIMULATOR=1 DEBUG=1 -C .

ipk: ts-M1M3supportd m1m3sscli ts-M1M3support_${VERSION}_x64.ipk

TS_DDSCONFIG=../ts_ddsconfig

ts-M1M3support_$(VERSION)_x64.ipk: ts-M1M3supportd m1m3sscli
	@echo '[MK ] ipk $@'
	${co}mkdir -p ipk/data/usr/sbin
	${co}mkdir -p ipk/data/etc/init.d
	${co}mkdir -p ipk/data/etc/default
	${co}mkdir -p ipk/data/var/lib/M1M3support
	${co}mkdir -p ipk/control
	${co}cp ts-M1M3supportd ipk/data/usr/sbin/ts-M1M3supportd
	${co}cp m1m3sscli ipk/data/usr/sbin/m1m3sscli
	${co}cp init ipk/data/etc/init.d/ts-M1M3support
	${co}cp default_M1M3support ipk/data/etc/default/M1M3support
	${co}cp -r ${TS_DDSCONFIG}/python/lsst/ts/ddsconfig/data/config/ospl-embedded-shmem.xml ipk/data/var/lib/M1M3support || echo "Cannot find ${TS_DDSCONFIG} ospl-embedded-shmem.xml - check it out?"
	${co}cp -r ${TS_DDSCONFIG}/python/lsst/ts/ddsconfig/data/qos/QoS.xml ipk/data/var/lib/M1M3support || echo "Cannot find ${TS_DDSCONFIG} QoS.xml - check it out?"
	${co}cp -r SettingFiles/* ipk/data/var/lib/M1M3support
	${co}cp -r Bitfiles/* ipk/data/var/lib/M1M3support
	${co}sed s?@VERSION@?$(VERSION)?g control.ipk.in > ipk/control/control
	${co}cp postinst prerm postrm ipk/control
	${co}echo -e "/etc/default/M1M3support\n/var/lib/M1M3support/ospl-embedded-shmem.xml\n/var/lib/M1M3supportS/QoS.xml" > ipk/control/conffiles
	${co}find SettingFiles -name '*.yaml' -o -name '*.csv' | sed 's#^SettingFiles#/var/lib/M1M3support#' >> ipk/control/conffiles
	${co}echo "2.0" > ipk/debian-binary
	${co}tar czf ipk/data.tar.gz -P --transform "s#^ipk/data#.#" --owner=0 --group=0 ipk/data
	${co}tar czf ipk/control.tar.gz -P --transform "s#^ipk/control#.#" --owner=0 --group=0 ipk/control
	${co}ar r $@ ipk/control.tar.gz ipk/data.tar.gz ipk/debian-binary
