
// g++ -L/home/ccontaxis/ts_sal/test/m1m3/cpp/src -Wall /home/ccontaxis/ts_sal/test/m1m3/cpp/src/uselibSALm1m3.cpp -lSALm1m3

#ifndef _LIB_SAL_M1M3_H_
#define _LIB_SAL_M1M3_H_

typedef signed char int8_t;
typedef short int int16_t;
typedef int int32_t;
typedef long int int64_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long int uint64_t;
typedef unsigned char bool_t;

struct m1m3_AirDataC;

extern "C" {
    
extern void initializeSAL();

extern int32_t salTelemetryPub(char* topic);

extern int32_t salTelemetrySub(char* topic);

extern int32_t salProcessor(char* topic);

extern int32_t salCommand(char* topic);

extern int32_t salEvent(char* topic);

extern int32_t putSampleAirData(m1m3_AirDataC* telemetryAirData);

extern int32_t getNextSampleAirData(m1m3_AirDataC* telemetryAirData);

extern void shutdownSAL();

}

#endif