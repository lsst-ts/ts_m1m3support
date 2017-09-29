
#include "SAL_m1m3.h"
#include "ccpp_sal_m1m3.h"
#include "libSALm1m3.h"

SAL_m1m3 m1m3SAL = SAL_m1m3();

extern "C" void initializeSAL() { m1m3SAL = SAL_m1m3(); }

extern "C" int32_t salTelemetryPub(char* topic) { return m1m3SAL.salTelemetryPub(topic); }

extern "C" int32_t salTelemetrySub(char* topic) { return m1m3SAL.salTelemetrySub(topic); }

extern "C" int32_t salProcessor(char* topic) { return m1m3SAL.salProcessor(topic); }

extern "C" int32_t salCommand(char* topic) { return m1m3SAL.salCommand(topic); }

extern "C" int32_t salEvent(char* topic) { return m1m3SAL.salEvent(topic); }

extern "C" int32_t putSampleAirData(m1m3_AirDataC* telemetryAirData) { return m1m3SAL.putSample_AirData(telemetryAirData); }

extern "C" int32_t getNextSampleAirData(m1m3_AirDataC* telemetryAirData) { return m1m3SAL.getNextSample_AirData(telemetryAirData); }

extern "C" void shutdownSAL() { m1m3SAL.salShutdown(); }