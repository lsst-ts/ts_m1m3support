
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
#define MESSAGE_TOPIC_ForceActuatorData 1
#define MESSAGE_TOPIC_MirrorForceData 2
#define MESSAGE_TOPIC_HardpointData 3
#define MESSAGE_TOPIC_AirData 4
#define MESSAGE_TOPIC_DynamicData 5
#define MESSAGE_TOPIC_FPGAData 6
#define MESSAGE_TOPIC_IMSData 7
#define MESSAGE_TOPIC_ForceActuatorStatus 8
#define MESSAGE_TOPIC_ElevationData 9
#define MESSAGE_TOPIC_HardpointStatus 10
#define MESSAGE_TOPIC_HardpointActuatorChase 11
#define MESSAGE_TOPIC_AirStatus 12
#define MESSAGE_TOPIC_ForceActuatorSafetyChecks 13
#define MESSAGE_TOPIC_ForceActuatorTest 14
#define MESSAGE_TOPIC_ActuatorBroadcastCounter 15
#define MESSAGE_TOPIC_StatusChecks 16
#define MESSAGE_TOPIC_CellChecks 17
#define MESSAGE_TOPIC_HardpointActuatorBreakaway 18
#define MESSAGE_TOPIC_RaiseMirrorComplete 19
#define MESSAGE_TOPIC_ILCCommunication 20
#define MESSAGE_TOPIC_ElevationAngleChecks 21
#define MESSAGE_TOPIC_DetailedState 22
#define MESSAGE_TOPIC_ServoLoops 23
#define MESSAGE_TOPIC_ForceActuatorInfo 24
#define MESSAGE_TOPIC_LowerMirrorComplete 25
#define MESSAGE_TOPIC_HardpointActuatorInfo 26
#define MESSAGE_TOPIC_SettingsApplied 27
#define MESSAGE_TOPIC_ErrorCode 28
#define MESSAGE_TOPIC_StartupChecks 29
#define MESSAGE_TOPIC_SummaryState 30
#define MESSAGE_TOPIC_SettingVersions 31
#define MESSAGE_TOPIC_ActuatorTestStatus 32
#define MESSAGE_TOPIC_AppliedSettingsMatchStart 33
#define MESSAGE_TOPIC_Enable 34
#define MESSAGE_TOPIC_EnterMaintenance 35
#define MESSAGE_TOPIC_Standby 36
#define MESSAGE_TOPIC_AbortLowerM1M3 37
#define MESSAGE_TOPIC_ApplyAberrationByForces 38
#define MESSAGE_TOPIC_ApplyAOSCorrectionByBendingModes 39
#define MESSAGE_TOPIC_ClearAOSCorrection 40
#define MESSAGE_TOPIC_Start 41
#define MESSAGE_TOPIC_RaiseM1M3 42
#define MESSAGE_TOPIC_TestForceActuator 43
#define MESSAGE_TOPIC_LowerM1M3 44
#define MESSAGE_TOPIC_ApplyAOSCorrectionByForces 45
#define MESSAGE_TOPIC_Disable 46
#define MESSAGE_TOPIC_ApplyAberrationByBendingModes 47
#define MESSAGE_TOPIC_TestHardpoint 48
#define MESSAGE_TOPIC_ManipulateM1M3 49
#define MESSAGE_TOPIC_ClearAberration 50
#define MESSAGE_TOPIC_Exit 51
#define MESSAGE_TOPIC_TestAir 52
#define MESSAGE_TOPIC_AbortRaiseM1M3 53
#define MESSAGE_TOPIC_MoveHardpointActuators 54
#define MESSAGE_TOPIC_ExitMaintenance 55
#define MESSAGE_TOPIC_Shutdown 56

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
SAL_m1m3 m1m3SAL = SAL_m1m3();
m1m3_ForceActuatorDataC telemetryForceActuatorDataData;
m1m3_MirrorForceDataC telemetryMirrorForceDataData;
m1m3_HardpointDataC telemetryHardpointDataData;
m1m3_AirDataC telemetryAirDataData;
m1m3_DynamicDataC telemetryDynamicDataData;
m1m3_FPGADataC telemetryFPGADataData;
m1m3_IMSDataC telemetryIMSDataData;
m1m3_ForceActuatorStatusC telemetryForceActuatorStatusData;
m1m3_ElevationDataC telemetryElevationDataData;
m1m3_HardpointStatusC telemetryHardpointStatusData;
m1m3_logevent_HardpointActuatorChaseC eventHardpointActuatorChaseData;
m1m3_logevent_AirStatusC eventAirStatusData;
m1m3_logevent_ForceActuatorSafetyChecksC eventForceActuatorSafetyChecksData;
m1m3_logevent_ForceActuatorTestC eventForceActuatorTestData;
m1m3_logevent_ActuatorBroadcastCounterC eventActuatorBroadcastCounterData;
m1m3_logevent_StatusChecksC eventStatusChecksData;
m1m3_logevent_CellChecksC eventCellChecksData;
m1m3_logevent_HardpointActuatorBreakawayC eventHardpointActuatorBreakawayData;
m1m3_logevent_RaiseMirrorCompleteC eventRaiseMirrorCompleteData;
m1m3_logevent_ILCCommunicationC eventILCCommunicationData;
m1m3_logevent_ElevationAngleChecksC eventElevationAngleChecksData;
m1m3_logevent_DetailedStateC eventDetailedStateData;
m1m3_logevent_ServoLoopsC eventServoLoopsData;
m1m3_logevent_ForceActuatorInfoC eventForceActuatorInfoData;
m1m3_logevent_LowerMirrorCompleteC eventLowerMirrorCompleteData;
m1m3_logevent_HardpointActuatorInfoC eventHardpointActuatorInfoData;
m1m3_logevent_SettingsAppliedC eventSettingsAppliedData;
m1m3_logevent_ErrorCodeC eventErrorCodeData;
m1m3_logevent_StartupChecksC eventStartupChecksData;
m1m3_logevent_SummaryStateC eventSummaryStateData;
m1m3_logevent_SettingVersionsC eventSettingVersionsData;
m1m3_logevent_ActuatorTestStatusC eventActuatorTestStatusData;
m1m3_logevent_AppliedSettingsMatchStartC eventAppliedSettingsMatchStartData;
m1m3_command_EnableC commandEnableData;
m1m3_command_EnterMaintenanceC commandEnterMaintenanceData;
m1m3_command_StandbyC commandStandbyData;
m1m3_command_AbortLowerM1M3C commandAbortLowerM1M3Data;
m1m3_command_ApplyAberrationByForcesC commandApplyAberrationByForcesData;
m1m3_command_ApplyAOSCorrectionByBendingModesC commandApplyAOSCorrectionByBendingModesData;
m1m3_command_ClearAOSCorrectionC commandClearAOSCorrectionData;
m1m3_command_StartC commandStartData;
m1m3_command_RaiseM1M3C commandRaiseM1M3Data;
m1m3_command_TestForceActuatorC commandTestForceActuatorData;
m1m3_command_LowerM1M3C commandLowerM1M3Data;
m1m3_command_ApplyAOSCorrectionByForcesC commandApplyAOSCorrectionByForcesData;
m1m3_command_DisableC commandDisableData;
m1m3_command_ApplyAberrationByBendingModesC commandApplyAberrationByBendingModesData;
m1m3_command_TestHardpointC commandTestHardpointData;
m1m3_command_ManipulateM1M3C commandManipulateM1M3Data;
m1m3_command_ClearAberrationC commandClearAberrationData;
m1m3_command_ExitC commandExitData;
m1m3_command_TestAirC commandTestAirData;
m1m3_command_AbortRaiseM1M3C commandAbortRaiseM1M3Data;
m1m3_command_MoveHardpointActuatorsC commandMoveHardpointActuatorsData;
m1m3_command_ExitMaintenanceC commandExitMaintenanceData;
m1m3_command_ShutdownC commandShutdownData;


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
        printf("GOT %d %d %d\n", messageHeader.type, messageHeader.topic, messageHeader.size);
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
    printf("Writing header {%d, %d, %d}\n", type, topic, size);
    headerIndex = 0;
    setU8(headerBuffer, &headerIndex, type);
    setU8(headerBuffer, &headerIndex, topic);
    setI32(headerBuffer, &headerIndex, size);
    return writeBuffer(headerBuffer, sizeof(MessageHeader));
}


void processPutSampleForceActuatorDataClientRequest() {
    printf("ForceActuatorData\n");
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorData]) {
        bufferIndex = 0;
        telemetryForceActuatorDataData.Timestamp = getDBL(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorDataData.SlewFlagCommanded[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorDataData.XSetpointCommanded[i] = getDBL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorDataData.YSetpointCommanded[i] = getDBL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorDataData.ZSetpointCommanded[i] = getDBL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorDataData.XForce[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorDataData.YForce[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorDataData.ZForce[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorDataData.ILCFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorDataData.DCAFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorDataData.BroadcastCounter[i] = getU8(buffer, &bufferIndex); }

        m1m3SAL.putSample_ForceActuatorData(&telemetryForceActuatorDataData);












    }
}


void processPutSampleMirrorForceDataClientRequest() {
    printf("MirrorForceData\n");
    if (enabledTopics[MESSAGE_TOPIC_MirrorForceData]) {
        bufferIndex = 0;
        telemetryMirrorForceDataData.Timestamp = getDBL(buffer, &bufferIndex);
        telemetryMirrorForceDataData.Fx = getDBL(buffer, &bufferIndex);
        telemetryMirrorForceDataData.Fy = getDBL(buffer, &bufferIndex);
        telemetryMirrorForceDataData.Fz = getDBL(buffer, &bufferIndex);
        telemetryMirrorForceDataData.Mx = getDBL(buffer, &bufferIndex);
        telemetryMirrorForceDataData.My = getDBL(buffer, &bufferIndex);
        telemetryMirrorForceDataData.Mz = getDBL(buffer, &bufferIndex);

        m1m3SAL.putSample_MirrorForceData(&telemetryMirrorForceDataData);








    }
}


void processPutSampleHardpointDataClientRequest() {
    printf("HardpointData\n");
    if (enabledTopics[MESSAGE_TOPIC_HardpointData]) {
        bufferIndex = 0;
        telemetryHardpointDataData.Timestamp = getDBL(buffer, &bufferIndex);
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointDataData.StepsCommanded[i] = getI16(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointDataData.Force[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointDataData.Encoder[i] = getI32(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointDataData.ILCFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointDataData.CWLimitOperated[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointDataData.CCWLimitOperated[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointDataData.BroadcastCounter[i] = getU8(buffer, &bufferIndex); }

        m1m3SAL.putSample_HardpointData(&telemetryHardpointDataData);









    }
}


void processPutSampleAirDataClientRequest() {
    printf("AirData\n");
    if (enabledTopics[MESSAGE_TOPIC_AirData]) {
        bufferIndex = 0;
        telemetryAirDataData.Timestamp = getDBL(buffer, &bufferIndex);
        telemetryAirDataData.CellAirPressure = getSGL(buffer, &bufferIndex);
        telemetryAirDataData.HardpointAirPressure = getSGL(buffer, &bufferIndex);

        m1m3SAL.putSample_AirData(&telemetryAirDataData);




    }
}


void processPutSampleDynamicDataClientRequest() {
    printf("DynamicData\n");
    if (enabledTopics[MESSAGE_TOPIC_DynamicData]) {
        bufferIndex = 0;
        telemetryDynamicDataData.Timestamp = getDBL(buffer, &bufferIndex);
        telemetryDynamicDataData.AzimuthVelocity = getDBL(buffer, &bufferIndex);
        telemetryDynamicDataData.ElevationVelocity = getDBL(buffer, &bufferIndex);
        telemetryDynamicDataData.AzimuthAcceleration = getDBL(buffer, &bufferIndex);
        telemetryDynamicDataData.ElevationAcceleration = getDBL(buffer, &bufferIndex);

        m1m3SAL.putSample_DynamicData(&telemetryDynamicDataData);






    }
}


void processPutSampleFPGADataClientRequest() {
    printf("FPGAData\n");
    if (enabledTopics[MESSAGE_TOPIC_FPGAData]) {
        bufferIndex = 0;
        telemetryFPGADataData.Timestamp = getDBL(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetATxInternalFIFOOverflow = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetAInvalidInstruction = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetAWaitForRxFrameTimeout = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetAStartBitError = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetAStopBitError = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetARxDataFIFOOverflow = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetATxByteCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetATxFrameCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetARxByteCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetARxFrameCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetBTxInternalFIFOOverflow = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetBInvalidInstruction = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetBWaitForRxFrameTimeout = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetBStartBitError = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetBStopBitError = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetBRxDataFIFOOverflow = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetBTxByteCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetBTxFrameCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetBRxByteCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetBRxFrameCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetCTxInternalFIFOOverflow = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetCInvalidInstruction = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetCWaitForRxFrameTimeout = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetCStartBitError = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetCStopBitError = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetCRxDataFIFOOverflow = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetCTxByteCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetCTxFrameCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetCRxByteCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetCRxFrameCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetDTxInternalFIFOOverflow = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetDInvalidInstruction = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetDWaitForRxFrameTimeout = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetDStartBitError = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetDStopBitError = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetDRxDataFIFOOverflow = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetDTxByteCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetDTxFrameCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetDRxByteCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetDRxFrameCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetETxInternalFIFOOverflow = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetEInvalidInstruction = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetEWaitForRxFrameTimeout = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetEStartBitError = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetEStopBitError = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetERxDataFIFOOverflow = getU8(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetETxByteCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetETxFrameCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetERxByteCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.ModbusSubnetERxFrameCount = getU32(buffer, &bufferIndex);
        telemetryFPGADataData.FPGATime = getDBL(buffer, &bufferIndex);

        m1m3SAL.putSample_FPGAData(&telemetryFPGADataData);





















































    }
}


void processPutSampleIMSDataClientRequest() {
    printf("IMSData\n");
    if (enabledTopics[MESSAGE_TOPIC_IMSData]) {
        bufferIndex = 0;
        telemetryIMSDataData.Timestamp = getDBL(buffer, &bufferIndex);
        telemetryIMSDataData.XPosition = getDBL(buffer, &bufferIndex);
        telemetryIMSDataData.YPosition = getDBL(buffer, &bufferIndex);
        telemetryIMSDataData.ZPosition = getDBL(buffer, &bufferIndex);
        telemetryIMSDataData.XRotation = getDBL(buffer, &bufferIndex);
        telemetryIMSDataData.YRotation = getDBL(buffer, &bufferIndex);
        telemetryIMSDataData.ZRotation = getDBL(buffer, &bufferIndex);

        m1m3SAL.putSample_IMSData(&telemetryIMSDataData);








    }
}


void processPutSampleForceActuatorStatusClientRequest() {
    printf("ForceActuatorStatus\n");
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorStatus]) {
        bufferIndex = 0;
        telemetryForceActuatorStatusData.Timestamp = getDBL(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.Mode[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.MajorFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.MinorFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.FaultOverridden[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.DCAFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.WatchdogReset[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.BrownoutDetected[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.EventTrapReset[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.MotorPowerFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.SSRPowerFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.AUXPowerFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.SMCPowerFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.DCAOutputsEnabled[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.DCAPowerFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.DCAAmplifierAFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.DCAAmplifierBFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { telemetryForceActuatorStatusData.DCAEventTrapReset[i] = getU8(buffer, &bufferIndex); }

        m1m3SAL.putSample_ForceActuatorStatus(&telemetryForceActuatorStatusData);



















    }
}


void processPutSampleElevationDataClientRequest() {
    printf("ElevationData\n");
    if (enabledTopics[MESSAGE_TOPIC_ElevationData]) {
        bufferIndex = 0;
        telemetryElevationDataData.Timestamp = getDBL(buffer, &bufferIndex);
        telemetryElevationDataData.InclinometerElevation = getDBL(buffer, &bufferIndex);
        telemetryElevationDataData.MeasuredElevation = getDBL(buffer, &bufferIndex);

        m1m3SAL.putSample_ElevationData(&telemetryElevationDataData);




    }
}


void processPutSampleHardpointStatusClientRequest() {
    printf("HardpointStatus\n");
    if (enabledTopics[MESSAGE_TOPIC_HardpointStatus]) {
        bufferIndex = 0;
        telemetryHardpointStatusData.Timestamp = getDBL(buffer, &bufferIndex);
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.Mode[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.MajorFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.MinorFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.FaultOverridden[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.WatchdogReset[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.BrownoutDetected[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.EventTrapReset[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.MotorPowerFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.SSRPowerFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.AUXPowerFault[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { telemetryHardpointStatusData.SMCPowerFault[i] = getU8(buffer, &bufferIndex); }

        m1m3SAL.putSample_HardpointStatus(&telemetryHardpointStatusData);













    }
}



void processPutSampleClientRequest() {
    printf("PUT_SAMPLE ");
    switch(messageHeader.topic) {
        case MESSAGE_TOPIC_ForceActuatorData: processPutSampleForceActuatorDataClientRequest(); break;
        case MESSAGE_TOPIC_MirrorForceData: processPutSampleMirrorForceDataClientRequest(); break;
        case MESSAGE_TOPIC_HardpointData: processPutSampleHardpointDataClientRequest(); break;
        case MESSAGE_TOPIC_AirData: processPutSampleAirDataClientRequest(); break;
        case MESSAGE_TOPIC_DynamicData: processPutSampleDynamicDataClientRequest(); break;
        case MESSAGE_TOPIC_FPGAData: processPutSampleFPGADataClientRequest(); break;
        case MESSAGE_TOPIC_IMSData: processPutSampleIMSDataClientRequest(); break;
        case MESSAGE_TOPIC_ForceActuatorStatus: processPutSampleForceActuatorStatusClientRequest(); break;
        case MESSAGE_TOPIC_ElevationData: processPutSampleElevationDataClientRequest(); break;
        case MESSAGE_TOPIC_HardpointStatus: processPutSampleHardpointStatusClientRequest(); break;

        default:
            printf("Unknown message header topic %d\n", messageHeader.topic);
            break;
    }
}

void processGetNextSampleClientRequest() {
    printf("GET_NEXT_SAMPLE ");
    switch(messageHeader.topic) {
        default:
            printf("Unknown message header topic %d\n", messageHeader.topic);
            break;
    }
}


void processLogEventHardpointActuatorChaseClientRequest() {
    printf("HardpointActuatorChase\n");
    if (enabledTopics[MESSAGE_TOPIC_HardpointActuatorChase]) {
        bufferIndex = 0;
        eventHardpointActuatorChaseData.Timestamp = getDBL(buffer, &bufferIndex);
        eventHardpointActuatorChaseData.ActuatorId = getI32(buffer, &bufferIndex);
        eventHardpointActuatorChaseData.Chasing = getU8(buffer, &bufferIndex);

        eventHardpointActuatorChaseData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_HardpointActuatorChase(&eventHardpointActuatorChaseData, eventHardpointActuatorChaseData.priority);




    }
}


void processLogEventAirStatusClientRequest() {
    printf("AirStatus\n");
    if (enabledTopics[MESSAGE_TOPIC_AirStatus]) {
        bufferIndex = 0;
        eventAirStatusData.Timestamp = getDBL(buffer, &bufferIndex);
        eventAirStatusData.AirCommandedOn = getU8(buffer, &bufferIndex);
        eventAirStatusData.AirOn = getU8(buffer, &bufferIndex);
        eventAirStatusData.AirPressureOk = getU8(buffer, &bufferIndex);
        eventAirStatusData.HardpointAirPressureOk = getU8(buffer, &bufferIndex);

        eventAirStatusData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_AirStatus(&eventAirStatusData, eventAirStatusData.priority);






    }
}


void processLogEventForceActuatorSafetyChecksClientRequest() {
    printf("ForceActuatorSafetyChecks\n");
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorSafetyChecks]) {
        bufferIndex = 0;
        eventForceActuatorSafetyChecksData.Timestamp = getDBL(buffer, &bufferIndex);
        eventForceActuatorSafetyChecksData.AllInSafetyLimit = getU8(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorSafetyChecksData.SafetyLimitOk[i] = getU8(buffer, &bufferIndex); }
        eventForceActuatorSafetyChecksData.MirrorWeightOk = getU8(buffer, &bufferIndex);
        eventForceActuatorSafetyChecksData.XMomentOk = getU8(buffer, &bufferIndex);
        eventForceActuatorSafetyChecksData.YMomentOk = getU8(buffer, &bufferIndex);
        eventForceActuatorSafetyChecksData.AllNeighborsOk = getU8(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorSafetyChecksData.NeighborsOk[i] = getU8(buffer, &bufferIndex); }
        eventForceActuatorSafetyChecksData.MagnitudeOK = getU8(buffer, &bufferIndex);
        eventForceActuatorSafetyChecksData.AllGlobalBendingOk = getU8(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorSafetyChecksData.GlobalBendingOk[i] = getU8(buffer, &bufferIndex); }
        eventForceActuatorSafetyChecksData.FollowingErrorOk = getU8(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorSafetyChecksData.AxialFollowingErrorOk[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorSafetyChecksData.LateralFollowingErrorOk[i] = getU8(buffer, &bufferIndex); }
        eventForceActuatorSafetyChecksData.AOSNetForceOk = getU8(buffer, &bufferIndex);
        eventForceActuatorSafetyChecksData.AllElevationForcesOk = getU8(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorSafetyChecksData.ElevationForcesOk[i] = getU8(buffer, &bufferIndex); }
        eventForceActuatorSafetyChecksData.AllAzimuthForcesOk = getU8(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorSafetyChecksData.AzimuthForcesOk[i] = getU8(buffer, &bufferIndex); }
        eventForceActuatorSafetyChecksData.AllThermalForcesOk = getU8(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorSafetyChecksData.ThermalForcesOk[i] = getU8(buffer, &bufferIndex); }
        eventForceActuatorSafetyChecksData.AllAOSForcesOk = getU8(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorSafetyChecksData.AOSForcesOk[i] = getU8(buffer, &bufferIndex); }

        eventForceActuatorSafetyChecksData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_ForceActuatorSafetyChecks(&eventForceActuatorSafetyChecksData, eventForceActuatorSafetyChecksData.priority);
























    }
}


void processLogEventForceActuatorTestClientRequest() {
    printf("ForceActuatorTest\n");
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorTest]) {
        bufferIndex = 0;
        eventForceActuatorTestData.Timestamp = getDBL(buffer, &bufferIndex);
        eventForceActuatorTestData.ActuatorId = getI32(buffer, &bufferIndex);
        eventForceActuatorTestData.TestStatus = getI32(buffer, &bufferIndex);

        eventForceActuatorTestData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_ForceActuatorTest(&eventForceActuatorTestData, eventForceActuatorTestData.priority);




    }
}


void processLogEventActuatorBroadcastCounterClientRequest() {
    printf("ActuatorBroadcastCounter\n");
    if (enabledTopics[MESSAGE_TOPIC_ActuatorBroadcastCounter]) {
        bufferIndex = 0;
        eventActuatorBroadcastCounterData.Timestamp = getDBL(buffer, &bufferIndex);
        eventActuatorBroadcastCounterData.ActuatorId = getI32(buffer, &bufferIndex);
        eventActuatorBroadcastCounterData.CounterOk = getU8(buffer, &bufferIndex);

        eventActuatorBroadcastCounterData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_ActuatorBroadcastCounter(&eventActuatorBroadcastCounterData, eventActuatorBroadcastCounterData.priority);




    }
}


void processLogEventStatusChecksClientRequest() {
    printf("StatusChecks\n");
    if (enabledTopics[MESSAGE_TOPIC_StatusChecks]) {
        bufferIndex = 0;
        eventStatusChecksData.Timestamp = getDBL(buffer, &bufferIndex);
        eventStatusChecksData.ILCResetOk = getU8(buffer, &bufferIndex);
        eventStatusChecksData.DCAResetOk = getU8(buffer, &bufferIndex);
        eventStatusChecksData.ILCPowerOk = getU8(buffer, &bufferIndex);
        eventStatusChecksData.DCAPowerOk = getU8(buffer, &bufferIndex);

        eventStatusChecksData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_StatusChecks(&eventStatusChecksData, eventStatusChecksData.priority);






    }
}


void processLogEventCellChecksClientRequest() {
    printf("CellChecks\n");
    if (enabledTopics[MESSAGE_TOPIC_CellChecks]) {
        bufferIndex = 0;
        eventCellChecksData.Timestamp = getDBL(buffer, &bufferIndex);
        eventCellChecksData.MirrorDoorClosed = getU8(buffer, &bufferIndex);

        eventCellChecksData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_CellChecks(&eventCellChecksData, eventCellChecksData.priority);



    }
}


void processLogEventHardpointActuatorBreakawayClientRequest() {
    printf("HardpointActuatorBreakaway\n");
    if (enabledTopics[MESSAGE_TOPIC_HardpointActuatorBreakaway]) {
        bufferIndex = 0;
        eventHardpointActuatorBreakawayData.Timestamp = getDBL(buffer, &bufferIndex);
        eventHardpointActuatorBreakawayData.ActuatorId = getI32(buffer, &bufferIndex);
        eventHardpointActuatorBreakawayData.ForceOk = getU8(buffer, &bufferIndex);

        eventHardpointActuatorBreakawayData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_HardpointActuatorBreakaway(&eventHardpointActuatorBreakawayData, eventHardpointActuatorBreakawayData.priority);




    }
}


void processLogEventRaiseMirrorCompleteClientRequest() {
    printf("RaiseMirrorComplete\n");
    if (enabledTopics[MESSAGE_TOPIC_RaiseMirrorComplete]) {
        bufferIndex = 0;
        eventRaiseMirrorCompleteData.Timestamp = getDBL(buffer, &bufferIndex);
        eventRaiseMirrorCompleteData.Successful = getU8(buffer, &bufferIndex);

        eventRaiseMirrorCompleteData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_RaiseMirrorComplete(&eventRaiseMirrorCompleteData, eventRaiseMirrorCompleteData.priority);



    }
}


void processLogEventILCCommunicationClientRequest() {
    printf("ILCCommunication\n");
    if (enabledTopics[MESSAGE_TOPIC_ILCCommunication]) {
        bufferIndex = 0;
        eventILCCommunicationData.Timestamp = getDBL(buffer, &bufferIndex);
        eventILCCommunicationData.ActuatorId = getI32(buffer, &bufferIndex);
        eventILCCommunicationData.CommunicationOk = getU8(buffer, &bufferIndex);

        eventILCCommunicationData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_ILCCommunication(&eventILCCommunicationData, eventILCCommunicationData.priority);




    }
}


void processLogEventElevationAngleChecksClientRequest() {
    printf("ElevationAngleChecks\n");
    if (enabledTopics[MESSAGE_TOPIC_ElevationAngleChecks]) {
        bufferIndex = 0;
        eventElevationAngleChecksData.Timestamp = getDBL(buffer, &bufferIndex);
        eventElevationAngleChecksData.MatchesTMA = getU8(buffer, &bufferIndex);
        eventElevationAngleChecksData.MatchesInclinometer = getU8(buffer, &bufferIndex);
        eventElevationAngleChecksData.MatchesMeasured = getU8(buffer, &bufferIndex);

        eventElevationAngleChecksData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_ElevationAngleChecks(&eventElevationAngleChecksData, eventElevationAngleChecksData.priority);





    }
}


void processLogEventDetailedStateClientRequest() {
    printf("DetailedState\n");
    if (enabledTopics[MESSAGE_TOPIC_DetailedState]) {
        bufferIndex = 0;
        eventDetailedStateData.Timestamp = getDBL(buffer, &bufferIndex);
        eventDetailedStateData.DetailedState = getI32(buffer, &bufferIndex);

        eventDetailedStateData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_DetailedState(&eventDetailedStateData, eventDetailedStateData.priority);



    }
}


void processLogEventServoLoopsClientRequest() {
    printf("ServoLoops\n");
    if (enabledTopics[MESSAGE_TOPIC_ServoLoops]) {
        bufferIndex = 0;
        eventServoLoopsData.Timestamp = getDBL(buffer, &bufferIndex);
        eventServoLoopsData.HardpointChase = getU8(buffer, &bufferIndex);
        eventServoLoopsData.HardpointOffloading = getU8(buffer, &bufferIndex);
        eventServoLoopsData.OuterLoopTimeOk = getU8(buffer, &bufferIndex);

        eventServoLoopsData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_ServoLoops(&eventServoLoopsData, eventServoLoopsData.priority);





    }
}


void processLogEventForceActuatorInfoClientRequest() {
    printf("ForceActuatorInfo\n");
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorInfo]) {
        bufferIndex = 0;
        eventForceActuatorInfoData.Timestamp = getDBL(buffer, &bufferIndex);
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.ReferenceId[i] = getI32(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.XPosition[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.YPosition[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.ZPosition[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.ILCUniqueId[i] = getI64(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.ILCApplicationType[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.NetworkNodeType[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.ILCSelectedOptions[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.NetworkNodeOptions[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MajorRevision[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MinorRevision[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.ADCScanRate[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MainADCCalibrationK1[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MainADCCalibrationK2[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MainADCCalibrationK3[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MainADCCalibrationK4[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MainAxialLoadCellOffset[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MainLateralLoadCellOffset[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MainAxialLoadCellSensitivity[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MainLateralLoadCellSensitivity[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.BackupADCCalibrationK1[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.BackupADCCalibrationK2[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.BackupADCCalibrationK3[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.BackupADCCalibrationK4[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.BackupAxialLoadCellOffset[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.BackupLateralLoadCellOffset[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.BackupAxialLoadCellSensitivity[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.BackupLateralLoadCellSensitivity[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCAAxialGain[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCALateralGain[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCAUniqueId[i] = getI64(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCAFirmwareType[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCAMajorRevision[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCAMinorRevision[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.MainCalibrationError[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.BackupCalibrationError[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCAFirmwareUpdate[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.UniqueIdCRCError[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.ApplicationTypeMismatch[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.ApplicationMissing[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.ApplicationCRCMismatch[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.OneWireMissing[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.OneWire1Mismatch[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.OneWire2Mismatch[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCAUniqueIdCRCError[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCAMainCalibrationError[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCABackupCalibrationError[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCAApplicationMissing[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCAApplicationCRCMismatch[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 156; i++) { eventForceActuatorInfoData.DCABootloaderActive[i] = getU8(buffer, &bufferIndex); }

        eventForceActuatorInfoData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_ForceActuatorInfo(&eventForceActuatorInfoData, eventForceActuatorInfoData.priority);




















































    }
}


void processLogEventLowerMirrorCompleteClientRequest() {
    printf("LowerMirrorComplete\n");
    if (enabledTopics[MESSAGE_TOPIC_LowerMirrorComplete]) {
        bufferIndex = 0;
        eventLowerMirrorCompleteData.Timestamp = getDBL(buffer, &bufferIndex);
        eventLowerMirrorCompleteData.Successful = getU8(buffer, &bufferIndex);

        eventLowerMirrorCompleteData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_LowerMirrorComplete(&eventLowerMirrorCompleteData, eventLowerMirrorCompleteData.priority);



    }
}


void processLogEventHardpointActuatorInfoClientRequest() {
    printf("HardpointActuatorInfo\n");
    if (enabledTopics[MESSAGE_TOPIC_HardpointActuatorInfo]) {
        bufferIndex = 0;
        eventHardpointActuatorInfoData.Timestamp = getDBL(buffer, &bufferIndex);
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.ReferenceId[i] = getI32(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.ReferencePosition[i] = getI32(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.XPosition[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.YPosition[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.ZPosition[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.ILCUniqueId[i] = getI64(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.ILCApplicationType[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.NetworkNodeType[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.ILCSelectedOptions[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.NetworkNodeOptions[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.MajorRevision[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.MinorRevision[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.ADCScanRate[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.MainADCCalibrationK1[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.MainADCCalibrationK2[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.MainADCCalibrationK3[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.MainADCCalibrationK4[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.MainLoadCellOffset[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.MainLoadCellSensitivity[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.BackupADCCalibrationK1[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.BackupADCCalibrationK2[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.BackupADCCalibrationK3[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.BackupADCCalibrationK4[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.BackupLoadCellOffset[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.BackupLoadCellSensitivity[i] = getSGL(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.MainCalibrationError[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.BackupCalibrationError[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.UniqueIdCRCError[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.ApplicationTypeMismatch[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.ApplicationMissing[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.ApplicationCRCMismatch[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.OneWireMissing[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.OneWire1Mismatch[i] = getU8(buffer, &bufferIndex); }
        for(int32_t i = 0; i < 6; i++) { eventHardpointActuatorInfoData.OneWire2Mismatch[i] = getU8(buffer, &bufferIndex); }

        eventHardpointActuatorInfoData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_HardpointActuatorInfo(&eventHardpointActuatorInfoData, eventHardpointActuatorInfoData.priority);




































    }
}


void processLogEventSettingsAppliedClientRequest() {
    printf("SettingsApplied\n");
    if (enabledTopics[MESSAGE_TOPIC_SettingsApplied]) {
        bufferIndex = 0;
        eventSettingsAppliedData.Timestamp = getDBL(buffer, &bufferIndex);
        std::string* tmp_Settings = getString(buffer, &bufferIndex); eventSettingsAppliedData.Settings = *tmp_Settings;

        eventSettingsAppliedData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_SettingsApplied(&eventSettingsAppliedData, eventSettingsAppliedData.priority);

        eventSettingsAppliedData.Settings = ""; delete tmp_Settings;

    }
}


void processLogEventErrorCodeClientRequest() {
    printf("ErrorCode\n");
    if (enabledTopics[MESSAGE_TOPIC_ErrorCode]) {
        bufferIndex = 0;
        eventErrorCodeData.Timestamp = getDBL(buffer, &bufferIndex);
        eventErrorCodeData.ErrorCode = getI32(buffer, &bufferIndex);

        eventErrorCodeData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_ErrorCode(&eventErrorCodeData, eventErrorCodeData.priority);



    }
}


void processLogEventStartupChecksClientRequest() {
    printf("StartupChecks\n");
    if (enabledTopics[MESSAGE_TOPIC_StartupChecks]) {
        bufferIndex = 0;
        eventStartupChecksData.Timestamp = getDBL(buffer, &bufferIndex);
        eventStartupChecksData.ILCVersionsOk = getU8(buffer, &bufferIndex);
        eventStartupChecksData.DCAVersionsOk = getU8(buffer, &bufferIndex);
        eventStartupChecksData.ILCFirmwareOk = getU8(buffer, &bufferIndex);
        eventStartupChecksData.DCAFirmwareOk = getU8(buffer, &bufferIndex);
        eventStartupChecksData.ILCStatusOk = getU8(buffer, &bufferIndex);
        eventStartupChecksData.DCAStatusOk = getU8(buffer, &bufferIndex);

        eventStartupChecksData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_StartupChecks(&eventStartupChecksData, eventStartupChecksData.priority);








    }
}


void processLogEventSummaryStateClientRequest() {
    printf("SummaryState\n");
    if (enabledTopics[MESSAGE_TOPIC_SummaryState]) {
        bufferIndex = 0;
        eventSummaryStateData.Timestamp = getDBL(buffer, &bufferIndex);
        eventSummaryStateData.SummaryState = getI32(buffer, &bufferIndex);

        eventSummaryStateData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_SummaryState(&eventSummaryStateData, eventSummaryStateData.priority);



    }
}


void processLogEventSettingVersionsClientRequest() {
    printf("SettingVersions\n");
    if (enabledTopics[MESSAGE_TOPIC_SettingVersions]) {
        bufferIndex = 0;
        eventSettingVersionsData.Timestamp = getDBL(buffer, &bufferIndex);
        std::string* tmp_RecommendedSettingsVersion = getString(buffer, &bufferIndex); eventSettingVersionsData.RecommendedSettingsVersion = *tmp_RecommendedSettingsVersion;

        eventSettingVersionsData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_SettingVersions(&eventSettingVersionsData, eventSettingVersionsData.priority);

        eventSettingVersionsData.RecommendedSettingsVersion = ""; delete tmp_RecommendedSettingsVersion;

    }
}


void processLogEventActuatorTestStatusClientRequest() {
    printf("ActuatorTestStatus\n");
    if (enabledTopics[MESSAGE_TOPIC_ActuatorTestStatus]) {
        bufferIndex = 0;
        eventActuatorTestStatusData.Timestamp = getDBL(buffer, &bufferIndex);
        eventActuatorTestStatusData.AllTestsRan = getU8(buffer, &bufferIndex);

        eventActuatorTestStatusData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_ActuatorTestStatus(&eventActuatorTestStatusData, eventActuatorTestStatusData.priority);



    }
}


void processLogEventAppliedSettingsMatchStartClientRequest() {
    printf("AppliedSettingsMatchStart\n");
    if (enabledTopics[MESSAGE_TOPIC_AppliedSettingsMatchStart]) {
        bufferIndex = 0;
        eventAppliedSettingsMatchStartData.Timestamp = getDBL(buffer, &bufferIndex);
        eventAppliedSettingsMatchStartData.AppliedSettingsMatchStart = getU8(buffer, &bufferIndex);

        eventAppliedSettingsMatchStartData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_AppliedSettingsMatchStart(&eventAppliedSettingsMatchStartData, eventAppliedSettingsMatchStartData.priority);



    }
}



void processLogEventClientRequest() {
    printf("LOG_EVENT ");
    switch(messageHeader.topic) {
        case MESSAGE_TOPIC_HardpointActuatorChase: processLogEventHardpointActuatorChaseClientRequest(); break;
        case MESSAGE_TOPIC_AirStatus: processLogEventAirStatusClientRequest(); break;
        case MESSAGE_TOPIC_ForceActuatorSafetyChecks: processLogEventForceActuatorSafetyChecksClientRequest(); break;
        case MESSAGE_TOPIC_ForceActuatorTest: processLogEventForceActuatorTestClientRequest(); break;
        case MESSAGE_TOPIC_ActuatorBroadcastCounter: processLogEventActuatorBroadcastCounterClientRequest(); break;
        case MESSAGE_TOPIC_StatusChecks: processLogEventStatusChecksClientRequest(); break;
        case MESSAGE_TOPIC_CellChecks: processLogEventCellChecksClientRequest(); break;
        case MESSAGE_TOPIC_HardpointActuatorBreakaway: processLogEventHardpointActuatorBreakawayClientRequest(); break;
        case MESSAGE_TOPIC_RaiseMirrorComplete: processLogEventRaiseMirrorCompleteClientRequest(); break;
        case MESSAGE_TOPIC_ILCCommunication: processLogEventILCCommunicationClientRequest(); break;
        case MESSAGE_TOPIC_ElevationAngleChecks: processLogEventElevationAngleChecksClientRequest(); break;
        case MESSAGE_TOPIC_DetailedState: processLogEventDetailedStateClientRequest(); break;
        case MESSAGE_TOPIC_ServoLoops: processLogEventServoLoopsClientRequest(); break;
        case MESSAGE_TOPIC_ForceActuatorInfo: processLogEventForceActuatorInfoClientRequest(); break;
        case MESSAGE_TOPIC_LowerMirrorComplete: processLogEventLowerMirrorCompleteClientRequest(); break;
        case MESSAGE_TOPIC_HardpointActuatorInfo: processLogEventHardpointActuatorInfoClientRequest(); break;
        case MESSAGE_TOPIC_SettingsApplied: processLogEventSettingsAppliedClientRequest(); break;
        case MESSAGE_TOPIC_ErrorCode: processLogEventErrorCodeClientRequest(); break;
        case MESSAGE_TOPIC_StartupChecks: processLogEventStartupChecksClientRequest(); break;
        case MESSAGE_TOPIC_SummaryState: processLogEventSummaryStateClientRequest(); break;
        case MESSAGE_TOPIC_SettingVersions: processLogEventSettingVersionsClientRequest(); break;
        case MESSAGE_TOPIC_ActuatorTestStatus: processLogEventActuatorTestStatusClientRequest(); break;
        case MESSAGE_TOPIC_AppliedSettingsMatchStart: processLogEventAppliedSettingsMatchStartClientRequest(); break;

        default:
            printf("Unknown message header topic %d\n", messageHeader.topic);
            break;
    }
}

void processGetEventClientRequest() {
    printf("GET_EVENT ");
    switch(messageHeader.topic) {
        default:
            printf("Unknown message header topic %d\n", messageHeader.topic);
            break;
    }    
}

void sendCommandId(uint8_t topic) {
    printf("COMMAND_ID %d\n", commandId);
    bufferIndex = 0;
    setI32(buffer, &bufferIndex, commandId);
    writeClientHeader(MESSAGE_TYPE_COMMAND_ID, topic, bufferIndex);
    writeBuffer(buffer, bufferIndex);
}


void processIssueCommandEnableClientRequest() {
    printf("Enable\n");
    if (enabledTopics[MESSAGE_TOPIC_Enable]) {
        bufferIndex = 0;
        commandEnableData.Enable = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_Enable(&commandEnableData);


        sendCommandId(MESSAGE_TOPIC_Enable);
    }
}


void processIssueCommandEnterMaintenanceClientRequest() {
    printf("EnterMaintenance\n");
    if (enabledTopics[MESSAGE_TOPIC_EnterMaintenance]) {
        bufferIndex = 0;
        commandEnterMaintenanceData.EnterMaintenance = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_EnterMaintenance(&commandEnterMaintenanceData);


        sendCommandId(MESSAGE_TOPIC_EnterMaintenance);
    }
}


void processIssueCommandStandbyClientRequest() {
    printf("Standby\n");
    if (enabledTopics[MESSAGE_TOPIC_Standby]) {
        bufferIndex = 0;
        commandStandbyData.Standby = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_Standby(&commandStandbyData);


        sendCommandId(MESSAGE_TOPIC_Standby);
    }
}


void processIssueCommandAbortLowerM1M3ClientRequest() {
    printf("AbortLowerM1M3\n");
    if (enabledTopics[MESSAGE_TOPIC_AbortLowerM1M3]) {
        bufferIndex = 0;
        commandAbortLowerM1M3Data.AbortLowerM1M3 = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_AbortLowerM1M3(&commandAbortLowerM1M3Data);


        sendCommandId(MESSAGE_TOPIC_AbortLowerM1M3);
    }
}


void processIssueCommandApplyAberrationByForcesClientRequest() {
    printf("ApplyAberrationByForces\n");
    if (enabledTopics[MESSAGE_TOPIC_ApplyAberrationByForces]) {
        bufferIndex = 0;
        for(int32_t i = 0; i < 156; i++) { commandApplyAberrationByForcesData.ZForces[i] = getDBL(buffer, &bufferIndex); }

        commandId = m1m3SAL.issueCommand_ApplyAberrationByForces(&commandApplyAberrationByForcesData);


        sendCommandId(MESSAGE_TOPIC_ApplyAberrationByForces);
    }
}


void processIssueCommandApplyAOSCorrectionByBendingModesClientRequest() {
    printf("ApplyAOSCorrectionByBendingModes\n");
    if (enabledTopics[MESSAGE_TOPIC_ApplyAOSCorrectionByBendingModes]) {
        bufferIndex = 0;
        for(int32_t i = 0; i < 22; i++) { commandApplyAOSCorrectionByBendingModesData.Coefficients[i] = getDBL(buffer, &bufferIndex); }

        commandId = m1m3SAL.issueCommand_ApplyAOSCorrectionByBendingModes(&commandApplyAOSCorrectionByBendingModesData);


        sendCommandId(MESSAGE_TOPIC_ApplyAOSCorrectionByBendingModes);
    }
}


void processIssueCommandClearAOSCorrectionClientRequest() {
    printf("ClearAOSCorrection\n");
    if (enabledTopics[MESSAGE_TOPIC_ClearAOSCorrection]) {
        bufferIndex = 0;
        commandClearAOSCorrectionData.ClearAOSCorrection = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_ClearAOSCorrection(&commandClearAOSCorrectionData);


        sendCommandId(MESSAGE_TOPIC_ClearAOSCorrection);
    }
}


void processIssueCommandStartClientRequest() {
    printf("Start\n");
    if (enabledTopics[MESSAGE_TOPIC_Start]) {
        bufferIndex = 0;
        commandStartData.Start = getU8(buffer, &bufferIndex);
        std::string* tmp_SettingsToApply = getString(buffer, &bufferIndex); commandStartData.SettingsToApply = *tmp_SettingsToApply;

        commandId = m1m3SAL.issueCommand_Start(&commandStartData);

        commandStartData.SettingsToApply = ""; delete tmp_SettingsToApply;

        sendCommandId(MESSAGE_TOPIC_Start);
    }
}


void processIssueCommandRaiseM1M3ClientRequest() {
    printf("RaiseM1M3\n");
    if (enabledTopics[MESSAGE_TOPIC_RaiseM1M3]) {
        bufferIndex = 0;
        commandRaiseM1M3Data.RaiseM1M3 = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_RaiseM1M3(&commandRaiseM1M3Data);


        sendCommandId(MESSAGE_TOPIC_RaiseM1M3);
    }
}


void processIssueCommandTestForceActuatorClientRequest() {
    printf("TestForceActuator\n");
    if (enabledTopics[MESSAGE_TOPIC_TestForceActuator]) {
        bufferIndex = 0;
        commandTestForceActuatorData.ActuatorId = getI32(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_TestForceActuator(&commandTestForceActuatorData);


        sendCommandId(MESSAGE_TOPIC_TestForceActuator);
    }
}


void processIssueCommandLowerM1M3ClientRequest() {
    printf("LowerM1M3\n");
    if (enabledTopics[MESSAGE_TOPIC_LowerM1M3]) {
        bufferIndex = 0;
        commandLowerM1M3Data.LowerM1M3 = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_LowerM1M3(&commandLowerM1M3Data);


        sendCommandId(MESSAGE_TOPIC_LowerM1M3);
    }
}


void processIssueCommandApplyAOSCorrectionByForcesClientRequest() {
    printf("ApplyAOSCorrectionByForces\n");
    if (enabledTopics[MESSAGE_TOPIC_ApplyAOSCorrectionByForces]) {
        bufferIndex = 0;
        for(int32_t i = 0; i < 156; i++) { commandApplyAOSCorrectionByForcesData.ZForces[i] = getDBL(buffer, &bufferIndex); }

        commandId = m1m3SAL.issueCommand_ApplyAOSCorrectionByForces(&commandApplyAOSCorrectionByForcesData);


        sendCommandId(MESSAGE_TOPIC_ApplyAOSCorrectionByForces);
    }
}


void processIssueCommandDisableClientRequest() {
    printf("Disable\n");
    if (enabledTopics[MESSAGE_TOPIC_Disable]) {
        bufferIndex = 0;
        commandDisableData.Disable = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_Disable(&commandDisableData);


        sendCommandId(MESSAGE_TOPIC_Disable);
    }
}


void processIssueCommandApplyAberrationByBendingModesClientRequest() {
    printf("ApplyAberrationByBendingModes\n");
    if (enabledTopics[MESSAGE_TOPIC_ApplyAberrationByBendingModes]) {
        bufferIndex = 0;
        for(int32_t i = 0; i < 22; i++) { commandApplyAberrationByBendingModesData.Coefficients[i] = getDBL(buffer, &bufferIndex); }

        commandId = m1m3SAL.issueCommand_ApplyAberrationByBendingModes(&commandApplyAberrationByBendingModesData);


        sendCommandId(MESSAGE_TOPIC_ApplyAberrationByBendingModes);
    }
}


void processIssueCommandTestHardpointClientRequest() {
    printf("TestHardpoint\n");
    if (enabledTopics[MESSAGE_TOPIC_TestHardpoint]) {
        bufferIndex = 0;
        commandTestHardpointData.ActuatorId = getI32(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_TestHardpoint(&commandTestHardpointData);


        sendCommandId(MESSAGE_TOPIC_TestHardpoint);
    }
}


void processIssueCommandManipulateM1M3ClientRequest() {
    printf("ManipulateM1M3\n");
    if (enabledTopics[MESSAGE_TOPIC_ManipulateM1M3]) {
        bufferIndex = 0;
        commandManipulateM1M3Data.XTranslation = getDBL(buffer, &bufferIndex);
        commandManipulateM1M3Data.YTranslation = getDBL(buffer, &bufferIndex);
        commandManipulateM1M3Data.ZTranslation = getDBL(buffer, &bufferIndex);
        commandManipulateM1M3Data.XRotation = getDBL(buffer, &bufferIndex);
        commandManipulateM1M3Data.YRotation = getDBL(buffer, &bufferIndex);
        commandManipulateM1M3Data.ZRotation = getDBL(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_ManipulateM1M3(&commandManipulateM1M3Data);







        sendCommandId(MESSAGE_TOPIC_ManipulateM1M3);
    }
}


void processIssueCommandClearAberrationClientRequest() {
    printf("ClearAberration\n");
    if (enabledTopics[MESSAGE_TOPIC_ClearAberration]) {
        bufferIndex = 0;
        commandClearAberrationData.ClearAberration = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_ClearAberration(&commandClearAberrationData);


        sendCommandId(MESSAGE_TOPIC_ClearAberration);
    }
}


void processIssueCommandExitClientRequest() {
    printf("Exit\n");
    if (enabledTopics[MESSAGE_TOPIC_Exit]) {
        bufferIndex = 0;
        commandExitData.Exit = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_Exit(&commandExitData);


        sendCommandId(MESSAGE_TOPIC_Exit);
    }
}


void processIssueCommandTestAirClientRequest() {
    printf("TestAir\n");
    if (enabledTopics[MESSAGE_TOPIC_TestAir]) {
        bufferIndex = 0;
        commandTestAirData.TestAir = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_TestAir(&commandTestAirData);


        sendCommandId(MESSAGE_TOPIC_TestAir);
    }
}


void processIssueCommandAbortRaiseM1M3ClientRequest() {
    printf("AbortRaiseM1M3\n");
    if (enabledTopics[MESSAGE_TOPIC_AbortRaiseM1M3]) {
        bufferIndex = 0;
        commandAbortRaiseM1M3Data.AbortRaiseM1M3 = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_AbortRaiseM1M3(&commandAbortRaiseM1M3Data);


        sendCommandId(MESSAGE_TOPIC_AbortRaiseM1M3);
    }
}


void processIssueCommandMoveHardpointActuatorsClientRequest() {
    printf("MoveHardpointActuators\n");
    if (enabledTopics[MESSAGE_TOPIC_MoveHardpointActuators]) {
        bufferIndex = 0;
        for(int32_t i = 0; i < 6; i++) { commandMoveHardpointActuatorsData.Steps[i] = getU8(buffer, &bufferIndex); }

        commandId = m1m3SAL.issueCommand_MoveHardpointActuators(&commandMoveHardpointActuatorsData);


        sendCommandId(MESSAGE_TOPIC_MoveHardpointActuators);
    }
}


void processIssueCommandExitMaintenanceClientRequest() {
    printf("ExitMaintenance\n");
    if (enabledTopics[MESSAGE_TOPIC_ExitMaintenance]) {
        bufferIndex = 0;
        commandExitMaintenanceData.ExitMaintenance = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_ExitMaintenance(&commandExitMaintenanceData);


        sendCommandId(MESSAGE_TOPIC_ExitMaintenance);
    }
}


void processIssueCommandShutdownClientRequest() {
    printf("Shutdown\n");
    if (enabledTopics[MESSAGE_TOPIC_Shutdown]) {
        bufferIndex = 0;
        commandShutdownData.Shutdown = getU8(buffer, &bufferIndex);

        commandId = m1m3SAL.issueCommand_Shutdown(&commandShutdownData);


        sendCommandId(MESSAGE_TOPIC_Shutdown);
    }
}



void processIssueCommandClientRequest() {
    printf("ISSUE_COMMAND ");
    switch(messageHeader.topic) {
        case MESSAGE_TOPIC_Enable: processIssueCommandEnableClientRequest(); break;
        case MESSAGE_TOPIC_EnterMaintenance: processIssueCommandEnterMaintenanceClientRequest(); break;
        case MESSAGE_TOPIC_Standby: processIssueCommandStandbyClientRequest(); break;
        case MESSAGE_TOPIC_AbortLowerM1M3: processIssueCommandAbortLowerM1M3ClientRequest(); break;
        case MESSAGE_TOPIC_ApplyAberrationByForces: processIssueCommandApplyAberrationByForcesClientRequest(); break;
        case MESSAGE_TOPIC_ApplyAOSCorrectionByBendingModes: processIssueCommandApplyAOSCorrectionByBendingModesClientRequest(); break;
        case MESSAGE_TOPIC_ClearAOSCorrection: processIssueCommandClearAOSCorrectionClientRequest(); break;
        case MESSAGE_TOPIC_Start: processIssueCommandStartClientRequest(); break;
        case MESSAGE_TOPIC_RaiseM1M3: processIssueCommandRaiseM1M3ClientRequest(); break;
        case MESSAGE_TOPIC_TestForceActuator: processIssueCommandTestForceActuatorClientRequest(); break;
        case MESSAGE_TOPIC_LowerM1M3: processIssueCommandLowerM1M3ClientRequest(); break;
        case MESSAGE_TOPIC_ApplyAOSCorrectionByForces: processIssueCommandApplyAOSCorrectionByForcesClientRequest(); break;
        case MESSAGE_TOPIC_Disable: processIssueCommandDisableClientRequest(); break;
        case MESSAGE_TOPIC_ApplyAberrationByBendingModes: processIssueCommandApplyAberrationByBendingModesClientRequest(); break;
        case MESSAGE_TOPIC_TestHardpoint: processIssueCommandTestHardpointClientRequest(); break;
        case MESSAGE_TOPIC_ManipulateM1M3: processIssueCommandManipulateM1M3ClientRequest(); break;
        case MESSAGE_TOPIC_ClearAberration: processIssueCommandClearAberrationClientRequest(); break;
        case MESSAGE_TOPIC_Exit: processIssueCommandExitClientRequest(); break;
        case MESSAGE_TOPIC_TestAir: processIssueCommandTestAirClientRequest(); break;
        case MESSAGE_TOPIC_AbortRaiseM1M3: processIssueCommandAbortRaiseM1M3ClientRequest(); break;
        case MESSAGE_TOPIC_MoveHardpointActuators: processIssueCommandMoveHardpointActuatorsClientRequest(); break;
        case MESSAGE_TOPIC_ExitMaintenance: processIssueCommandExitMaintenanceClientRequest(); break;
        case MESSAGE_TOPIC_Shutdown: processIssueCommandShutdownClientRequest(); break;

        default:
            printf("Unknown message header topic %d\n", messageHeader.topic);
            break;
    }
}

void processCommandIdClientRequest() {
    printf("COMMAND_ID ");
    switch(messageHeader.topic) {
        default:
            printf("Unknown message header topic %d\n", messageHeader.topic);
            break;
    }
}

void processWaitForCompletionClientRequest() {
    printf("WAIT_FOR_COMPLETION ");
    switch(messageHeader.topic) {
        default:
            printf("Unknown message header topic %d\n", messageHeader.topic);
            break;
    }
}

void processAcceptCommandClientRequest() {
    printf("ACCEPT_COMMAND ");
    switch(messageHeader.topic) {
        default:
            printf("Unknown message header topic %d\n", messageHeader.topic);
            break;
    }
}

void processAckCommandClientRequest() {
    printf("ACK_COMMAND ");
    switch(messageHeader.topic) {
        default:
            printf("Uknown message header topic %d\n", messageHeader.topic);
            break;
    }
}

void processShutdownClientRequest() {
    printf("Shutting down SALPipe\n");
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
                                printf("Unknown message header type %d\n", messageHeader.type);
                                break;
                        }
                    }
                }
                break;
        }
    }
}


void processGetNextSampleForceActuatorDataSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorData]) {
        rc = m1m3SAL.getNextSample_ForceActuatorData(&telemetryForceActuatorDataData);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE ForceActuatorData\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, telemetryForceActuatorDataData.Timestamp);
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorDataData.SlewFlagCommanded[i]); }
        for(int32_t i = 0; i < 156; i++) { setDBL(buffer, &bufferIndex, telemetryForceActuatorDataData.XSetpointCommanded[i]); }
        for(int32_t i = 0; i < 156; i++) { setDBL(buffer, &bufferIndex, telemetryForceActuatorDataData.YSetpointCommanded[i]); }
        for(int32_t i = 0; i < 156; i++) { setDBL(buffer, &bufferIndex, telemetryForceActuatorDataData.ZSetpointCommanded[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, telemetryForceActuatorDataData.XForce[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, telemetryForceActuatorDataData.YForce[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, telemetryForceActuatorDataData.ZForce[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorDataData.ILCFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorDataData.DCAFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorDataData.BroadcastCounter[i]); }

            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_ForceActuatorData, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetNextSampleMirrorForceDataSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_MirrorForceData]) {
        rc = m1m3SAL.getNextSample_MirrorForceData(&telemetryMirrorForceDataData);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE MirrorForceData\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, telemetryMirrorForceDataData.Timestamp);
        setDBL(buffer, &bufferIndex, telemetryMirrorForceDataData.Fx);
        setDBL(buffer, &bufferIndex, telemetryMirrorForceDataData.Fy);
        setDBL(buffer, &bufferIndex, telemetryMirrorForceDataData.Fz);
        setDBL(buffer, &bufferIndex, telemetryMirrorForceDataData.Mx);
        setDBL(buffer, &bufferIndex, telemetryMirrorForceDataData.My);
        setDBL(buffer, &bufferIndex, telemetryMirrorForceDataData.Mz);

            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_MirrorForceData, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetNextSampleHardpointDataSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_HardpointData]) {
        rc = m1m3SAL.getNextSample_HardpointData(&telemetryHardpointDataData);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE HardpointData\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, telemetryHardpointDataData.Timestamp);
        for(int32_t i = 0; i < 6; i++) { setI16(buffer, &bufferIndex, telemetryHardpointDataData.StepsCommanded[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, telemetryHardpointDataData.Force[i]); }
        for(int32_t i = 0; i < 6; i++) { setI32(buffer, &bufferIndex, telemetryHardpointDataData.Encoder[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointDataData.ILCFault[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointDataData.CWLimitOperated[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointDataData.CCWLimitOperated[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointDataData.BroadcastCounter[i]); }

            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_HardpointData, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetNextSampleAirDataSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_AirData]) {
        rc = m1m3SAL.getNextSample_AirData(&telemetryAirDataData);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE AirData\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, telemetryAirDataData.Timestamp);
        setSGL(buffer, &bufferIndex, telemetryAirDataData.CellAirPressure);
        setSGL(buffer, &bufferIndex, telemetryAirDataData.HardpointAirPressure);

            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_AirData, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetNextSampleDynamicDataSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_DynamicData]) {
        rc = m1m3SAL.getNextSample_DynamicData(&telemetryDynamicDataData);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE DynamicData\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, telemetryDynamicDataData.Timestamp);
        setDBL(buffer, &bufferIndex, telemetryDynamicDataData.AzimuthVelocity);
        setDBL(buffer, &bufferIndex, telemetryDynamicDataData.ElevationVelocity);
        setDBL(buffer, &bufferIndex, telemetryDynamicDataData.AzimuthAcceleration);
        setDBL(buffer, &bufferIndex, telemetryDynamicDataData.ElevationAcceleration);

            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_DynamicData, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetNextSampleFPGADataSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_FPGAData]) {
        rc = m1m3SAL.getNextSample_FPGAData(&telemetryFPGADataData);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE FPGAData\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, telemetryFPGADataData.Timestamp);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetATxInternalFIFOOverflow);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetAInvalidInstruction);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetAWaitForRxFrameTimeout);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetAStartBitError);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetAStopBitError);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetARxDataFIFOOverflow);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetATxByteCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetATxFrameCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetARxByteCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetARxFrameCount);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetBTxInternalFIFOOverflow);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetBInvalidInstruction);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetBWaitForRxFrameTimeout);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetBStartBitError);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetBStopBitError);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetBRxDataFIFOOverflow);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetBTxByteCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetBTxFrameCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetBRxByteCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetBRxFrameCount);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetCTxInternalFIFOOverflow);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetCInvalidInstruction);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetCWaitForRxFrameTimeout);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetCStartBitError);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetCStopBitError);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetCRxDataFIFOOverflow);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetCTxByteCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetCTxFrameCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetCRxByteCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetCRxFrameCount);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetDTxInternalFIFOOverflow);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetDInvalidInstruction);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetDWaitForRxFrameTimeout);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetDStartBitError);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetDStopBitError);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetDRxDataFIFOOverflow);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetDTxByteCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetDTxFrameCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetDRxByteCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetDRxFrameCount);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetETxInternalFIFOOverflow);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetEInvalidInstruction);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetEWaitForRxFrameTimeout);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetEStartBitError);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetEStopBitError);
        setU8(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetERxDataFIFOOverflow);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetETxByteCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetETxFrameCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetERxByteCount);
        setU32(buffer, &bufferIndex, telemetryFPGADataData.ModbusSubnetERxFrameCount);
        setDBL(buffer, &bufferIndex, telemetryFPGADataData.FPGATime);

            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_FPGAData, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetNextSampleIMSDataSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_IMSData]) {
        rc = m1m3SAL.getNextSample_IMSData(&telemetryIMSDataData);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE IMSData\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, telemetryIMSDataData.Timestamp);
        setDBL(buffer, &bufferIndex, telemetryIMSDataData.XPosition);
        setDBL(buffer, &bufferIndex, telemetryIMSDataData.YPosition);
        setDBL(buffer, &bufferIndex, telemetryIMSDataData.ZPosition);
        setDBL(buffer, &bufferIndex, telemetryIMSDataData.XRotation);
        setDBL(buffer, &bufferIndex, telemetryIMSDataData.YRotation);
        setDBL(buffer, &bufferIndex, telemetryIMSDataData.ZRotation);

            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_IMSData, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetNextSampleForceActuatorStatusSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorStatus]) {
        rc = m1m3SAL.getNextSample_ForceActuatorStatus(&telemetryForceActuatorStatusData);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE ForceActuatorStatus\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, telemetryForceActuatorStatusData.Timestamp);
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.Mode[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.MajorFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.MinorFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.FaultOverridden[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.DCAFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.WatchdogReset[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.BrownoutDetected[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.EventTrapReset[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.MotorPowerFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.SSRPowerFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.AUXPowerFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.SMCPowerFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.DCAOutputsEnabled[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.DCAPowerFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.DCAAmplifierAFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.DCAAmplifierBFault[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, telemetryForceActuatorStatusData.DCAEventTrapReset[i]); }

            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_ForceActuatorStatus, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetNextSampleElevationDataSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ElevationData]) {
        rc = m1m3SAL.getNextSample_ElevationData(&telemetryElevationDataData);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE ElevationData\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, telemetryElevationDataData.Timestamp);
        setDBL(buffer, &bufferIndex, telemetryElevationDataData.InclinometerElevation);
        setDBL(buffer, &bufferIndex, telemetryElevationDataData.MeasuredElevation);

            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_ElevationData, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetNextSampleHardpointStatusSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_HardpointStatus]) {
        rc = m1m3SAL.getNextSample_HardpointStatus(&telemetryHardpointStatusData);
        if (rc == SAL__OK) {
            printf("GET_NEXT_SAMPLE HardpointStatus\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, telemetryHardpointStatusData.Timestamp);
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.Mode[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.MajorFault[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.MinorFault[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.FaultOverridden[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.WatchdogReset[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.BrownoutDetected[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.EventTrapReset[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.MotorPowerFault[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.SSRPowerFault[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.AUXPowerFault[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, telemetryHardpointStatusData.SMCPowerFault[i]); }

            writeClientHeader(MESSAGE_TYPE_GET_NEXT_SAMPLE, MESSAGE_TOPIC_HardpointStatus, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}


void processGetNextSampleSALRequest() {
        processGetNextSampleForceActuatorDataSALRequest();
        processGetNextSampleMirrorForceDataSALRequest();
        processGetNextSampleHardpointDataSALRequest();
        processGetNextSampleAirDataSALRequest();
        processGetNextSampleDynamicDataSALRequest();
        processGetNextSampleFPGADataSALRequest();
        processGetNextSampleIMSDataSALRequest();
        processGetNextSampleForceActuatorStatusSALRequest();
        processGetNextSampleElevationDataSALRequest();
        processGetNextSampleHardpointStatusSALRequest();

}


void processGetEventHardpointActuatorChaseSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_HardpointActuatorChase]) {
        rc = m1m3SAL.getEvent_HardpointActuatorChase(&eventHardpointActuatorChaseData);
        if (rc == SAL__OK) {
            printf("GET_EVENT HardpointActuatorChase\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventHardpointActuatorChaseData.Timestamp);
        setI32(buffer, &bufferIndex, eventHardpointActuatorChaseData.ActuatorId);
        setU8(buffer, &bufferIndex, eventHardpointActuatorChaseData.Chasing);

            setI32(buffer, &bufferIndex, eventHardpointActuatorChaseData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_HardpointActuatorChase, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventAirStatusSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_AirStatus]) {
        rc = m1m3SAL.getEvent_AirStatus(&eventAirStatusData);
        if (rc == SAL__OK) {
            printf("GET_EVENT AirStatus\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventAirStatusData.Timestamp);
        setU8(buffer, &bufferIndex, eventAirStatusData.AirCommandedOn);
        setU8(buffer, &bufferIndex, eventAirStatusData.AirOn);
        setU8(buffer, &bufferIndex, eventAirStatusData.AirPressureOk);
        setU8(buffer, &bufferIndex, eventAirStatusData.HardpointAirPressureOk);

            setI32(buffer, &bufferIndex, eventAirStatusData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_AirStatus, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventForceActuatorSafetyChecksSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorSafetyChecks]) {
        rc = m1m3SAL.getEvent_ForceActuatorSafetyChecks(&eventForceActuatorSafetyChecksData);
        if (rc == SAL__OK) {
            printf("GET_EVENT ForceActuatorSafetyChecks\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.Timestamp);
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AllInSafetyLimit);
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.SafetyLimitOk[i]); }
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.MirrorWeightOk);
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.XMomentOk);
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.YMomentOk);
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AllNeighborsOk);
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.NeighborsOk[i]); }
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.MagnitudeOK);
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AllGlobalBendingOk);
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.GlobalBendingOk[i]); }
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.FollowingErrorOk);
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AxialFollowingErrorOk[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.LateralFollowingErrorOk[i]); }
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AOSNetForceOk);
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AllElevationForcesOk);
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.ElevationForcesOk[i]); }
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AllAzimuthForcesOk);
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AzimuthForcesOk[i]); }
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AllThermalForcesOk);
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.ThermalForcesOk[i]); }
        setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AllAOSForcesOk);
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.AOSForcesOk[i]); }

            setI32(buffer, &bufferIndex, eventForceActuatorSafetyChecksData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_ForceActuatorSafetyChecks, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventForceActuatorTestSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorTest]) {
        rc = m1m3SAL.getEvent_ForceActuatorTest(&eventForceActuatorTestData);
        if (rc == SAL__OK) {
            printf("GET_EVENT ForceActuatorTest\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventForceActuatorTestData.Timestamp);
        setI32(buffer, &bufferIndex, eventForceActuatorTestData.ActuatorId);
        setI32(buffer, &bufferIndex, eventForceActuatorTestData.TestStatus);

            setI32(buffer, &bufferIndex, eventForceActuatorTestData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_ForceActuatorTest, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventActuatorBroadcastCounterSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ActuatorBroadcastCounter]) {
        rc = m1m3SAL.getEvent_ActuatorBroadcastCounter(&eventActuatorBroadcastCounterData);
        if (rc == SAL__OK) {
            printf("GET_EVENT ActuatorBroadcastCounter\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventActuatorBroadcastCounterData.Timestamp);
        setI32(buffer, &bufferIndex, eventActuatorBroadcastCounterData.ActuatorId);
        setU8(buffer, &bufferIndex, eventActuatorBroadcastCounterData.CounterOk);

            setI32(buffer, &bufferIndex, eventActuatorBroadcastCounterData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_ActuatorBroadcastCounter, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventStatusChecksSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_StatusChecks]) {
        rc = m1m3SAL.getEvent_StatusChecks(&eventStatusChecksData);
        if (rc == SAL__OK) {
            printf("GET_EVENT StatusChecks\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventStatusChecksData.Timestamp);
        setU8(buffer, &bufferIndex, eventStatusChecksData.ILCResetOk);
        setU8(buffer, &bufferIndex, eventStatusChecksData.DCAResetOk);
        setU8(buffer, &bufferIndex, eventStatusChecksData.ILCPowerOk);
        setU8(buffer, &bufferIndex, eventStatusChecksData.DCAPowerOk);

            setI32(buffer, &bufferIndex, eventStatusChecksData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_StatusChecks, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventCellChecksSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_CellChecks]) {
        rc = m1m3SAL.getEvent_CellChecks(&eventCellChecksData);
        if (rc == SAL__OK) {
            printf("GET_EVENT CellChecks\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventCellChecksData.Timestamp);
        setU8(buffer, &bufferIndex, eventCellChecksData.MirrorDoorClosed);

            setI32(buffer, &bufferIndex, eventCellChecksData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_CellChecks, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventHardpointActuatorBreakawaySALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_HardpointActuatorBreakaway]) {
        rc = m1m3SAL.getEvent_HardpointActuatorBreakaway(&eventHardpointActuatorBreakawayData);
        if (rc == SAL__OK) {
            printf("GET_EVENT HardpointActuatorBreakaway\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventHardpointActuatorBreakawayData.Timestamp);
        setI32(buffer, &bufferIndex, eventHardpointActuatorBreakawayData.ActuatorId);
        setU8(buffer, &bufferIndex, eventHardpointActuatorBreakawayData.ForceOk);

            setI32(buffer, &bufferIndex, eventHardpointActuatorBreakawayData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_HardpointActuatorBreakaway, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventRaiseMirrorCompleteSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_RaiseMirrorComplete]) {
        rc = m1m3SAL.getEvent_RaiseMirrorComplete(&eventRaiseMirrorCompleteData);
        if (rc == SAL__OK) {
            printf("GET_EVENT RaiseMirrorComplete\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventRaiseMirrorCompleteData.Timestamp);
        setU8(buffer, &bufferIndex, eventRaiseMirrorCompleteData.Successful);

            setI32(buffer, &bufferIndex, eventRaiseMirrorCompleteData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_RaiseMirrorComplete, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventILCCommunicationSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ILCCommunication]) {
        rc = m1m3SAL.getEvent_ILCCommunication(&eventILCCommunicationData);
        if (rc == SAL__OK) {
            printf("GET_EVENT ILCCommunication\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventILCCommunicationData.Timestamp);
        setI32(buffer, &bufferIndex, eventILCCommunicationData.ActuatorId);
        setU8(buffer, &bufferIndex, eventILCCommunicationData.CommunicationOk);

            setI32(buffer, &bufferIndex, eventILCCommunicationData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_ILCCommunication, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventElevationAngleChecksSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ElevationAngleChecks]) {
        rc = m1m3SAL.getEvent_ElevationAngleChecks(&eventElevationAngleChecksData);
        if (rc == SAL__OK) {
            printf("GET_EVENT ElevationAngleChecks\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventElevationAngleChecksData.Timestamp);
        setU8(buffer, &bufferIndex, eventElevationAngleChecksData.MatchesTMA);
        setU8(buffer, &bufferIndex, eventElevationAngleChecksData.MatchesInclinometer);
        setU8(buffer, &bufferIndex, eventElevationAngleChecksData.MatchesMeasured);

            setI32(buffer, &bufferIndex, eventElevationAngleChecksData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_ElevationAngleChecks, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventDetailedStateSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_DetailedState]) {
        rc = m1m3SAL.getEvent_DetailedState(&eventDetailedStateData);
        if (rc == SAL__OK) {
            printf("GET_EVENT DetailedState\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventDetailedStateData.Timestamp);
        setI32(buffer, &bufferIndex, eventDetailedStateData.DetailedState);

            setI32(buffer, &bufferIndex, eventDetailedStateData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_DetailedState, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventServoLoopsSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ServoLoops]) {
        rc = m1m3SAL.getEvent_ServoLoops(&eventServoLoopsData);
        if (rc == SAL__OK) {
            printf("GET_EVENT ServoLoops\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventServoLoopsData.Timestamp);
        setU8(buffer, &bufferIndex, eventServoLoopsData.HardpointChase);
        setU8(buffer, &bufferIndex, eventServoLoopsData.HardpointOffloading);
        setU8(buffer, &bufferIndex, eventServoLoopsData.OuterLoopTimeOk);

            setI32(buffer, &bufferIndex, eventServoLoopsData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_ServoLoops, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventForceActuatorInfoSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorInfo]) {
        rc = m1m3SAL.getEvent_ForceActuatorInfo(&eventForceActuatorInfoData);
        if (rc == SAL__OK) {
            printf("GET_EVENT ForceActuatorInfo\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventForceActuatorInfoData.Timestamp);
        for(int32_t i = 0; i < 156; i++) { setI32(buffer, &bufferIndex, eventForceActuatorInfoData.ReferenceId[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.XPosition[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.YPosition[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.ZPosition[i]); }
        for(int32_t i = 0; i < 156; i++) { setI64(buffer, &bufferIndex, eventForceActuatorInfoData.ILCUniqueId[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.ILCApplicationType[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.NetworkNodeType[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.ILCSelectedOptions[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.NetworkNodeOptions[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.MajorRevision[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.MinorRevision[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.ADCScanRate[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.MainADCCalibrationK1[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.MainADCCalibrationK2[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.MainADCCalibrationK3[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.MainADCCalibrationK4[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.MainAxialLoadCellOffset[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.MainLateralLoadCellOffset[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.MainAxialLoadCellSensitivity[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.MainLateralLoadCellSensitivity[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.BackupADCCalibrationK1[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.BackupADCCalibrationK2[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.BackupADCCalibrationK3[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.BackupADCCalibrationK4[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.BackupAxialLoadCellOffset[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.BackupLateralLoadCellOffset[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.BackupAxialLoadCellSensitivity[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.BackupLateralLoadCellSensitivity[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.DCAAxialGain[i]); }
        for(int32_t i = 0; i < 156; i++) { setSGL(buffer, &bufferIndex, eventForceActuatorInfoData.DCALateralGain[i]); }
        for(int32_t i = 0; i < 156; i++) { setI64(buffer, &bufferIndex, eventForceActuatorInfoData.DCAUniqueId[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.DCAFirmwareType[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.DCAMajorRevision[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.DCAMinorRevision[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.MainCalibrationError[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.BackupCalibrationError[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.DCAFirmwareUpdate[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.UniqueIdCRCError[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.ApplicationTypeMismatch[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.ApplicationMissing[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.ApplicationCRCMismatch[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.OneWireMissing[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.OneWire1Mismatch[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.OneWire2Mismatch[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.DCAUniqueIdCRCError[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.DCAMainCalibrationError[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.DCABackupCalibrationError[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.DCAApplicationMissing[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.DCAApplicationCRCMismatch[i]); }
        for(int32_t i = 0; i < 156; i++) { setU8(buffer, &bufferIndex, eventForceActuatorInfoData.DCABootloaderActive[i]); }

            setI32(buffer, &bufferIndex, eventForceActuatorInfoData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_ForceActuatorInfo, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventLowerMirrorCompleteSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_LowerMirrorComplete]) {
        rc = m1m3SAL.getEvent_LowerMirrorComplete(&eventLowerMirrorCompleteData);
        if (rc == SAL__OK) {
            printf("GET_EVENT LowerMirrorComplete\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventLowerMirrorCompleteData.Timestamp);
        setU8(buffer, &bufferIndex, eventLowerMirrorCompleteData.Successful);

            setI32(buffer, &bufferIndex, eventLowerMirrorCompleteData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_LowerMirrorComplete, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventHardpointActuatorInfoSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_HardpointActuatorInfo]) {
        rc = m1m3SAL.getEvent_HardpointActuatorInfo(&eventHardpointActuatorInfoData);
        if (rc == SAL__OK) {
            printf("GET_EVENT HardpointActuatorInfo\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventHardpointActuatorInfoData.Timestamp);
        for(int32_t i = 0; i < 6; i++) { setI32(buffer, &bufferIndex, eventHardpointActuatorInfoData.ReferenceId[i]); }
        for(int32_t i = 0; i < 6; i++) { setI32(buffer, &bufferIndex, eventHardpointActuatorInfoData.ReferencePosition[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.XPosition[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.YPosition[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.ZPosition[i]); }
        for(int32_t i = 0; i < 6; i++) { setI64(buffer, &bufferIndex, eventHardpointActuatorInfoData.ILCUniqueId[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.ILCApplicationType[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.NetworkNodeType[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.ILCSelectedOptions[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.NetworkNodeOptions[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.MajorRevision[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.MinorRevision[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.ADCScanRate[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.MainADCCalibrationK1[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.MainADCCalibrationK2[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.MainADCCalibrationK3[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.MainADCCalibrationK4[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.MainLoadCellOffset[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.MainLoadCellSensitivity[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.BackupADCCalibrationK1[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.BackupADCCalibrationK2[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.BackupADCCalibrationK3[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.BackupADCCalibrationK4[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.BackupLoadCellOffset[i]); }
        for(int32_t i = 0; i < 6; i++) { setSGL(buffer, &bufferIndex, eventHardpointActuatorInfoData.BackupLoadCellSensitivity[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.MainCalibrationError[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.BackupCalibrationError[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.UniqueIdCRCError[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.ApplicationTypeMismatch[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.ApplicationMissing[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.ApplicationCRCMismatch[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.OneWireMissing[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.OneWire1Mismatch[i]); }
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, eventHardpointActuatorInfoData.OneWire2Mismatch[i]); }

            setI32(buffer, &bufferIndex, eventHardpointActuatorInfoData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_HardpointActuatorInfo, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventSettingsAppliedSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_SettingsApplied]) {
        rc = m1m3SAL.getEvent_SettingsApplied(&eventSettingsAppliedData);
        if (rc == SAL__OK) {
            printf("GET_EVENT SettingsApplied\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventSettingsAppliedData.Timestamp);
        setString(buffer, &bufferIndex, &eventSettingsAppliedData.Settings);

            setI32(buffer, &bufferIndex, eventSettingsAppliedData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_SettingsApplied, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventErrorCodeSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ErrorCode]) {
        rc = m1m3SAL.getEvent_ErrorCode(&eventErrorCodeData);
        if (rc == SAL__OK) {
            printf("GET_EVENT ErrorCode\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventErrorCodeData.Timestamp);
        setI32(buffer, &bufferIndex, eventErrorCodeData.ErrorCode);

            setI32(buffer, &bufferIndex, eventErrorCodeData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_ErrorCode, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventStartupChecksSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_StartupChecks]) {
        rc = m1m3SAL.getEvent_StartupChecks(&eventStartupChecksData);
        if (rc == SAL__OK) {
            printf("GET_EVENT StartupChecks\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventStartupChecksData.Timestamp);
        setU8(buffer, &bufferIndex, eventStartupChecksData.ILCVersionsOk);
        setU8(buffer, &bufferIndex, eventStartupChecksData.DCAVersionsOk);
        setU8(buffer, &bufferIndex, eventStartupChecksData.ILCFirmwareOk);
        setU8(buffer, &bufferIndex, eventStartupChecksData.DCAFirmwareOk);
        setU8(buffer, &bufferIndex, eventStartupChecksData.ILCStatusOk);
        setU8(buffer, &bufferIndex, eventStartupChecksData.DCAStatusOk);

            setI32(buffer, &bufferIndex, eventStartupChecksData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_StartupChecks, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventSummaryStateSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_SummaryState]) {
        rc = m1m3SAL.getEvent_SummaryState(&eventSummaryStateData);
        if (rc == SAL__OK) {
            printf("GET_EVENT SummaryState\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventSummaryStateData.Timestamp);
        setI32(buffer, &bufferIndex, eventSummaryStateData.SummaryState);

            setI32(buffer, &bufferIndex, eventSummaryStateData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_SummaryState, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventSettingVersionsSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_SettingVersions]) {
        rc = m1m3SAL.getEvent_SettingVersions(&eventSettingVersionsData);
        if (rc == SAL__OK) {
            printf("GET_EVENT SettingVersions\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventSettingVersionsData.Timestamp);
        setString(buffer, &bufferIndex, &eventSettingVersionsData.RecommendedSettingsVersion);

            setI32(buffer, &bufferIndex, eventSettingVersionsData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_SettingVersions, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventActuatorTestStatusSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ActuatorTestStatus]) {
        rc = m1m3SAL.getEvent_ActuatorTestStatus(&eventActuatorTestStatusData);
        if (rc == SAL__OK) {
            printf("GET_EVENT ActuatorTestStatus\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventActuatorTestStatusData.Timestamp);
        setU8(buffer, &bufferIndex, eventActuatorTestStatusData.AllTestsRan);

            setI32(buffer, &bufferIndex, eventActuatorTestStatusData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_ActuatorTestStatus, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventAppliedSettingsMatchStartSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_AppliedSettingsMatchStart]) {
        rc = m1m3SAL.getEvent_AppliedSettingsMatchStart(&eventAppliedSettingsMatchStartData);
        if (rc == SAL__OK) {
            printf("GET_EVENT AppliedSettingsMatchStart\n");
            bufferIndex = 0;
        setDBL(buffer, &bufferIndex, eventAppliedSettingsMatchStartData.Timestamp);
        setU8(buffer, &bufferIndex, eventAppliedSettingsMatchStartData.AppliedSettingsMatchStart);

            setI32(buffer, &bufferIndex, eventAppliedSettingsMatchStartData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_AppliedSettingsMatchStart, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}


void processGetEventSALRequest() {
        processGetEventHardpointActuatorChaseSALRequest();
        processGetEventAirStatusSALRequest();
        processGetEventForceActuatorSafetyChecksSALRequest();
        processGetEventForceActuatorTestSALRequest();
        processGetEventActuatorBroadcastCounterSALRequest();
        processGetEventStatusChecksSALRequest();
        processGetEventCellChecksSALRequest();
        processGetEventHardpointActuatorBreakawaySALRequest();
        processGetEventRaiseMirrorCompleteSALRequest();
        processGetEventILCCommunicationSALRequest();
        processGetEventElevationAngleChecksSALRequest();
        processGetEventDetailedStateSALRequest();
        processGetEventServoLoopsSALRequest();
        processGetEventForceActuatorInfoSALRequest();
        processGetEventLowerMirrorCompleteSALRequest();
        processGetEventHardpointActuatorInfoSALRequest();
        processGetEventSettingsAppliedSALRequest();
        processGetEventErrorCodeSALRequest();
        processGetEventStartupChecksSALRequest();
        processGetEventSummaryStateSALRequest();
        processGetEventSettingVersionsSALRequest();
        processGetEventActuatorTestStatusSALRequest();
        processGetEventAppliedSettingsMatchStartSALRequest();

}


void processAcceptCommandEnableSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_Enable]) {
        commandId = m1m3SAL.acceptCommand_Enable(&commandEnableData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND Enable\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandEnableData.Enable);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_Enable, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandEnterMaintenanceSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_EnterMaintenance]) {
        commandId = m1m3SAL.acceptCommand_EnterMaintenance(&commandEnterMaintenanceData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND EnterMaintenance\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandEnterMaintenanceData.EnterMaintenance);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_EnterMaintenance, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandStandbySALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_Standby]) {
        commandId = m1m3SAL.acceptCommand_Standby(&commandStandbyData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND Standby\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandStandbyData.Standby);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_Standby, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandAbortLowerM1M3SALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_AbortLowerM1M3]) {
        commandId = m1m3SAL.acceptCommand_AbortLowerM1M3(&commandAbortLowerM1M3Data);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND AbortLowerM1M3\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandAbortLowerM1M3Data.AbortLowerM1M3);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_AbortLowerM1M3, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandApplyAberrationByForcesSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ApplyAberrationByForces]) {
        commandId = m1m3SAL.acceptCommand_ApplyAberrationByForces(&commandApplyAberrationByForcesData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND ApplyAberrationByForces\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        for(int32_t i = 0; i < 156; i++) { setDBL(buffer, &bufferIndex, commandApplyAberrationByForcesData.ZForces[i]); }

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_ApplyAberrationByForces, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandApplyAOSCorrectionByBendingModesSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ApplyAOSCorrectionByBendingModes]) {
        commandId = m1m3SAL.acceptCommand_ApplyAOSCorrectionByBendingModes(&commandApplyAOSCorrectionByBendingModesData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND ApplyAOSCorrectionByBendingModes\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        for(int32_t i = 0; i < 22; i++) { setDBL(buffer, &bufferIndex, commandApplyAOSCorrectionByBendingModesData.Coefficients[i]); }

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_ApplyAOSCorrectionByBendingModes, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandClearAOSCorrectionSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ClearAOSCorrection]) {
        commandId = m1m3SAL.acceptCommand_ClearAOSCorrection(&commandClearAOSCorrectionData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND ClearAOSCorrection\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandClearAOSCorrectionData.ClearAOSCorrection);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_ClearAOSCorrection, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandStartSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_Start]) {
        commandId = m1m3SAL.acceptCommand_Start(&commandStartData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND Start\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandStartData.Start);
        setString(buffer, &bufferIndex, &commandStartData.SettingsToApply);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_Start, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandRaiseM1M3SALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_RaiseM1M3]) {
        commandId = m1m3SAL.acceptCommand_RaiseM1M3(&commandRaiseM1M3Data);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND RaiseM1M3\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandRaiseM1M3Data.RaiseM1M3);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_RaiseM1M3, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandTestForceActuatorSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_TestForceActuator]) {
        commandId = m1m3SAL.acceptCommand_TestForceActuator(&commandTestForceActuatorData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND TestForceActuator\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setI32(buffer, &bufferIndex, commandTestForceActuatorData.ActuatorId);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_TestForceActuator, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandLowerM1M3SALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_LowerM1M3]) {
        commandId = m1m3SAL.acceptCommand_LowerM1M3(&commandLowerM1M3Data);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND LowerM1M3\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandLowerM1M3Data.LowerM1M3);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_LowerM1M3, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandApplyAOSCorrectionByForcesSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ApplyAOSCorrectionByForces]) {
        commandId = m1m3SAL.acceptCommand_ApplyAOSCorrectionByForces(&commandApplyAOSCorrectionByForcesData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND ApplyAOSCorrectionByForces\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        for(int32_t i = 0; i < 156; i++) { setDBL(buffer, &bufferIndex, commandApplyAOSCorrectionByForcesData.ZForces[i]); }

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_ApplyAOSCorrectionByForces, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandDisableSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_Disable]) {
        commandId = m1m3SAL.acceptCommand_Disable(&commandDisableData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND Disable\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandDisableData.Disable);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_Disable, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandApplyAberrationByBendingModesSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ApplyAberrationByBendingModes]) {
        commandId = m1m3SAL.acceptCommand_ApplyAberrationByBendingModes(&commandApplyAberrationByBendingModesData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND ApplyAberrationByBendingModes\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        for(int32_t i = 0; i < 22; i++) { setDBL(buffer, &bufferIndex, commandApplyAberrationByBendingModesData.Coefficients[i]); }

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_ApplyAberrationByBendingModes, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandTestHardpointSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_TestHardpoint]) {
        commandId = m1m3SAL.acceptCommand_TestHardpoint(&commandTestHardpointData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND TestHardpoint\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setI32(buffer, &bufferIndex, commandTestHardpointData.ActuatorId);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_TestHardpoint, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandManipulateM1M3SALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ManipulateM1M3]) {
        commandId = m1m3SAL.acceptCommand_ManipulateM1M3(&commandManipulateM1M3Data);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND ManipulateM1M3\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setDBL(buffer, &bufferIndex, commandManipulateM1M3Data.XTranslation);
        setDBL(buffer, &bufferIndex, commandManipulateM1M3Data.YTranslation);
        setDBL(buffer, &bufferIndex, commandManipulateM1M3Data.ZTranslation);
        setDBL(buffer, &bufferIndex, commandManipulateM1M3Data.XRotation);
        setDBL(buffer, &bufferIndex, commandManipulateM1M3Data.YRotation);
        setDBL(buffer, &bufferIndex, commandManipulateM1M3Data.ZRotation);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_ManipulateM1M3, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandClearAberrationSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ClearAberration]) {
        commandId = m1m3SAL.acceptCommand_ClearAberration(&commandClearAberrationData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND ClearAberration\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandClearAberrationData.ClearAberration);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_ClearAberration, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandExitSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_Exit]) {
        commandId = m1m3SAL.acceptCommand_Exit(&commandExitData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND Exit\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandExitData.Exit);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_Exit, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandTestAirSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_TestAir]) {
        commandId = m1m3SAL.acceptCommand_TestAir(&commandTestAirData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND TestAir\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandTestAirData.TestAir);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_TestAir, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandAbortRaiseM1M3SALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_AbortRaiseM1M3]) {
        commandId = m1m3SAL.acceptCommand_AbortRaiseM1M3(&commandAbortRaiseM1M3Data);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND AbortRaiseM1M3\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandAbortRaiseM1M3Data.AbortRaiseM1M3);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_AbortRaiseM1M3, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandMoveHardpointActuatorsSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_MoveHardpointActuators]) {
        commandId = m1m3SAL.acceptCommand_MoveHardpointActuators(&commandMoveHardpointActuatorsData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND MoveHardpointActuators\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        for(int32_t i = 0; i < 6; i++) { setU8(buffer, &bufferIndex, commandMoveHardpointActuatorsData.Steps[i]); }

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_MoveHardpointActuators, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandExitMaintenanceSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ExitMaintenance]) {
        commandId = m1m3SAL.acceptCommand_ExitMaintenance(&commandExitMaintenanceData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND ExitMaintenance\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandExitMaintenanceData.ExitMaintenance);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_ExitMaintenance, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandShutdownSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_Shutdown]) {
        commandId = m1m3SAL.acceptCommand_Shutdown(&commandShutdownData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND Shutdown\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
        setU8(buffer, &bufferIndex, commandShutdownData.Shutdown);

            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_Shutdown, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}


void processAcceptCommandSALRequest() {
    processAcceptCommandEnableSALRequest();
    processAcceptCommandEnterMaintenanceSALRequest();
    processAcceptCommandStandbySALRequest();
    processAcceptCommandAbortLowerM1M3SALRequest();
    processAcceptCommandApplyAberrationByForcesSALRequest();
    processAcceptCommandApplyAOSCorrectionByBendingModesSALRequest();
    processAcceptCommandClearAOSCorrectionSALRequest();
    processAcceptCommandStartSALRequest();
    processAcceptCommandRaiseM1M3SALRequest();
    processAcceptCommandTestForceActuatorSALRequest();
    processAcceptCommandLowerM1M3SALRequest();
    processAcceptCommandApplyAOSCorrectionByForcesSALRequest();
    processAcceptCommandDisableSALRequest();
    processAcceptCommandApplyAberrationByBendingModesSALRequest();
    processAcceptCommandTestHardpointSALRequest();
    processAcceptCommandManipulateM1M3SALRequest();
    processAcceptCommandClearAberrationSALRequest();
    processAcceptCommandExitSALRequest();
    processAcceptCommandTestAirSALRequest();
    processAcceptCommandAbortRaiseM1M3SALRequest();
    processAcceptCommandMoveHardpointActuatorsSALRequest();
    processAcceptCommandExitMaintenanceSALRequest();
    processAcceptCommandShutdownSALRequest();

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
            fprintf(stderr, "SALPIPE: Invalid port number.\n");
            exit(EXIT_FAILURE);
        }
        
        stayAlive = strtol(argumentValues[2], &endptr, 0);
        if (*endptr) {
            fprintf(stderr, "SALPIPE: Invalid stay alive.\n");
            exit(EXIT_FAILURE);
        }
        
        int enabledTopic = 0;
        for(int i = 3; i < argumentCount; i++) {
            enabledTopic = strtol(argumentValues[i], &endptr, 0);
            if (*endptr) {
                fprintf(stderr, "Invalid enabled topic.\n");
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
        fprintf(stderr, "SALPIPE: Invalid arguments.\n");
        exit(EXIT_FAILURE);
    }

    // Create listener handle
    if ((listenerHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fprintf(stderr, "SALPIPE: Error creating listening socket.\n");
        exit(EXIT_FAILURE);
    }

    // Create listener address
    listenerAddress.sin_family      = AF_INET;
    listenerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    listenerAddress.sin_port        = htons(port);

    // Bind listener
    if ( bind(listenerHandle, (struct sockaddr *) &listenerAddress, sizeof(listenerAddress)) < 0 ) {
        fprintf(stderr, "SALPIPE: Error calling bind()\n");
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
        fprintf(stderr, "SALPIPE: Error calling listen()\n");
        exit(EXIT_FAILURE);
    }
    
    // Wait for a client to connect
    printf("Waiting for client\n");
    if ((connectionHandle = accept(listenerHandle, NULL, NULL) ) < 0 ) {
        fprintf(stderr, "SALPIPE: Error calling accept()\n");
        exit(EXIT_FAILURE);
    }

    // Initialization Start
    printf("Initializing ");
    
    // Initialize Telemetry
    printf("Telemetry ");
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorData]) { m1m3SAL.salTelemetryPub("m1m3_ForceActuatorData"); m1m3SAL.salTelemetrySub("m1m3_ForceActuatorData"); }
    if (enabledTopics[MESSAGE_TOPIC_MirrorForceData]) { m1m3SAL.salTelemetryPub("m1m3_MirrorForceData"); m1m3SAL.salTelemetrySub("m1m3_MirrorForceData"); }
    if (enabledTopics[MESSAGE_TOPIC_HardpointData]) { m1m3SAL.salTelemetryPub("m1m3_HardpointData"); m1m3SAL.salTelemetrySub("m1m3_HardpointData"); }
    if (enabledTopics[MESSAGE_TOPIC_AirData]) { m1m3SAL.salTelemetryPub("m1m3_AirData"); m1m3SAL.salTelemetrySub("m1m3_AirData"); }
    if (enabledTopics[MESSAGE_TOPIC_DynamicData]) { m1m3SAL.salTelemetryPub("m1m3_DynamicData"); m1m3SAL.salTelemetrySub("m1m3_DynamicData"); }
    if (enabledTopics[MESSAGE_TOPIC_FPGAData]) { m1m3SAL.salTelemetryPub("m1m3_FPGAData"); m1m3SAL.salTelemetrySub("m1m3_FPGAData"); }
    if (enabledTopics[MESSAGE_TOPIC_IMSData]) { m1m3SAL.salTelemetryPub("m1m3_IMSData"); m1m3SAL.salTelemetrySub("m1m3_IMSData"); }
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorStatus]) { m1m3SAL.salTelemetryPub("m1m3_ForceActuatorStatus"); m1m3SAL.salTelemetrySub("m1m3_ForceActuatorStatus"); }
    if (enabledTopics[MESSAGE_TOPIC_ElevationData]) { m1m3SAL.salTelemetryPub("m1m3_ElevationData"); m1m3SAL.salTelemetrySub("m1m3_ElevationData"); }
    if (enabledTopics[MESSAGE_TOPIC_HardpointStatus]) { m1m3SAL.salTelemetryPub("m1m3_HardpointStatus"); m1m3SAL.salTelemetrySub("m1m3_HardpointStatus"); }

    
    // Initialize Events
    printf("Events ");
    if (enabledTopics[MESSAGE_TOPIC_HardpointActuatorChase]) { m1m3SAL.salEvent("m1m3_logevent_HardpointActuatorChase"); }
    if (enabledTopics[MESSAGE_TOPIC_AirStatus]) { m1m3SAL.salEvent("m1m3_logevent_AirStatus"); }
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorSafetyChecks]) { m1m3SAL.salEvent("m1m3_logevent_ForceActuatorSafetyChecks"); }
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorTest]) { m1m3SAL.salEvent("m1m3_logevent_ForceActuatorTest"); }
    if (enabledTopics[MESSAGE_TOPIC_ActuatorBroadcastCounter]) { m1m3SAL.salEvent("m1m3_logevent_ActuatorBroadcastCounter"); }
    if (enabledTopics[MESSAGE_TOPIC_StatusChecks]) { m1m3SAL.salEvent("m1m3_logevent_StatusChecks"); }
    if (enabledTopics[MESSAGE_TOPIC_CellChecks]) { m1m3SAL.salEvent("m1m3_logevent_CellChecks"); }
    if (enabledTopics[MESSAGE_TOPIC_HardpointActuatorBreakaway]) { m1m3SAL.salEvent("m1m3_logevent_HardpointActuatorBreakaway"); }
    if (enabledTopics[MESSAGE_TOPIC_RaiseMirrorComplete]) { m1m3SAL.salEvent("m1m3_logevent_RaiseMirrorComplete"); }
    if (enabledTopics[MESSAGE_TOPIC_ILCCommunication]) { m1m3SAL.salEvent("m1m3_logevent_ILCCommunication"); }
    if (enabledTopics[MESSAGE_TOPIC_ElevationAngleChecks]) { m1m3SAL.salEvent("m1m3_logevent_ElevationAngleChecks"); }
    if (enabledTopics[MESSAGE_TOPIC_DetailedState]) { m1m3SAL.salEvent("m1m3_logevent_DetailedState"); }
    if (enabledTopics[MESSAGE_TOPIC_ServoLoops]) { m1m3SAL.salEvent("m1m3_logevent_ServoLoops"); }
    if (enabledTopics[MESSAGE_TOPIC_ForceActuatorInfo]) { m1m3SAL.salEvent("m1m3_logevent_ForceActuatorInfo"); }
    if (enabledTopics[MESSAGE_TOPIC_LowerMirrorComplete]) { m1m3SAL.salEvent("m1m3_logevent_LowerMirrorComplete"); }
    if (enabledTopics[MESSAGE_TOPIC_HardpointActuatorInfo]) { m1m3SAL.salEvent("m1m3_logevent_HardpointActuatorInfo"); }
    if (enabledTopics[MESSAGE_TOPIC_SettingsApplied]) { m1m3SAL.salEvent("m1m3_logevent_SettingsApplied"); }
    if (enabledTopics[MESSAGE_TOPIC_ErrorCode]) { m1m3SAL.salEvent("m1m3_logevent_ErrorCode"); }
    if (enabledTopics[MESSAGE_TOPIC_StartupChecks]) { m1m3SAL.salEvent("m1m3_logevent_StartupChecks"); }
    if (enabledTopics[MESSAGE_TOPIC_SummaryState]) { m1m3SAL.salEvent("m1m3_logevent_SummaryState"); }
    if (enabledTopics[MESSAGE_TOPIC_SettingVersions]) { m1m3SAL.salEvent("m1m3_logevent_SettingVersions"); }
    if (enabledTopics[MESSAGE_TOPIC_ActuatorTestStatus]) { m1m3SAL.salEvent("m1m3_logevent_ActuatorTestStatus"); }
    if (enabledTopics[MESSAGE_TOPIC_AppliedSettingsMatchStart]) { m1m3SAL.salEvent("m1m3_logevent_AppliedSettingsMatchStart"); }

    
    // Initialize Commands
    printf("Commands ");
    if (enabledTopics[MESSAGE_TOPIC_Enable]) { m1m3SAL.salCommand("m1m3_command_Enable"); m1m3SAL.salProcessor("m1m3_command_Enable"); }
    if (enabledTopics[MESSAGE_TOPIC_EnterMaintenance]) { m1m3SAL.salCommand("m1m3_command_EnterMaintenance"); m1m3SAL.salProcessor("m1m3_command_EnterMaintenance"); }
    if (enabledTopics[MESSAGE_TOPIC_Standby]) { m1m3SAL.salCommand("m1m3_command_Standby"); m1m3SAL.salProcessor("m1m3_command_Standby"); }
    if (enabledTopics[MESSAGE_TOPIC_AbortLowerM1M3]) { m1m3SAL.salCommand("m1m3_command_AbortLowerM1M3"); m1m3SAL.salProcessor("m1m3_command_AbortLowerM1M3"); }
    if (enabledTopics[MESSAGE_TOPIC_ApplyAberrationByForces]) { m1m3SAL.salCommand("m1m3_command_ApplyAberrationByForces"); m1m3SAL.salProcessor("m1m3_command_ApplyAberrationByForces"); }
    if (enabledTopics[MESSAGE_TOPIC_ApplyAOSCorrectionByBendingModes]) { m1m3SAL.salCommand("m1m3_command_ApplyAOSCorrectionByBendingModes"); m1m3SAL.salProcessor("m1m3_command_ApplyAOSCorrectionByBendingModes"); }
    if (enabledTopics[MESSAGE_TOPIC_ClearAOSCorrection]) { m1m3SAL.salCommand("m1m3_command_ClearAOSCorrection"); m1m3SAL.salProcessor("m1m3_command_ClearAOSCorrection"); }
    if (enabledTopics[MESSAGE_TOPIC_Start]) { m1m3SAL.salCommand("m1m3_command_Start"); m1m3SAL.salProcessor("m1m3_command_Start"); }
    if (enabledTopics[MESSAGE_TOPIC_RaiseM1M3]) { m1m3SAL.salCommand("m1m3_command_RaiseM1M3"); m1m3SAL.salProcessor("m1m3_command_RaiseM1M3"); }
    if (enabledTopics[MESSAGE_TOPIC_TestForceActuator]) { m1m3SAL.salCommand("m1m3_command_TestForceActuator"); m1m3SAL.salProcessor("m1m3_command_TestForceActuator"); }
    if (enabledTopics[MESSAGE_TOPIC_LowerM1M3]) { m1m3SAL.salCommand("m1m3_command_LowerM1M3"); m1m3SAL.salProcessor("m1m3_command_LowerM1M3"); }
    if (enabledTopics[MESSAGE_TOPIC_ApplyAOSCorrectionByForces]) { m1m3SAL.salCommand("m1m3_command_ApplyAOSCorrectionByForces"); m1m3SAL.salProcessor("m1m3_command_ApplyAOSCorrectionByForces"); }
    if (enabledTopics[MESSAGE_TOPIC_Disable]) { m1m3SAL.salCommand("m1m3_command_Disable"); m1m3SAL.salProcessor("m1m3_command_Disable"); }
    if (enabledTopics[MESSAGE_TOPIC_ApplyAberrationByBendingModes]) { m1m3SAL.salCommand("m1m3_command_ApplyAberrationByBendingModes"); m1m3SAL.salProcessor("m1m3_command_ApplyAberrationByBendingModes"); }
    if (enabledTopics[MESSAGE_TOPIC_TestHardpoint]) { m1m3SAL.salCommand("m1m3_command_TestHardpoint"); m1m3SAL.salProcessor("m1m3_command_TestHardpoint"); }
    if (enabledTopics[MESSAGE_TOPIC_ManipulateM1M3]) { m1m3SAL.salCommand("m1m3_command_ManipulateM1M3"); m1m3SAL.salProcessor("m1m3_command_ManipulateM1M3"); }
    if (enabledTopics[MESSAGE_TOPIC_ClearAberration]) { m1m3SAL.salCommand("m1m3_command_ClearAberration"); m1m3SAL.salProcessor("m1m3_command_ClearAberration"); }
    if (enabledTopics[MESSAGE_TOPIC_Exit]) { m1m3SAL.salCommand("m1m3_command_Exit"); m1m3SAL.salProcessor("m1m3_command_Exit"); }
    if (enabledTopics[MESSAGE_TOPIC_TestAir]) { m1m3SAL.salCommand("m1m3_command_TestAir"); m1m3SAL.salProcessor("m1m3_command_TestAir"); }
    if (enabledTopics[MESSAGE_TOPIC_AbortRaiseM1M3]) { m1m3SAL.salCommand("m1m3_command_AbortRaiseM1M3"); m1m3SAL.salProcessor("m1m3_command_AbortRaiseM1M3"); }
    if (enabledTopics[MESSAGE_TOPIC_MoveHardpointActuators]) { m1m3SAL.salCommand("m1m3_command_MoveHardpointActuators"); m1m3SAL.salProcessor("m1m3_command_MoveHardpointActuators"); }
    if (enabledTopics[MESSAGE_TOPIC_ExitMaintenance]) { m1m3SAL.salCommand("m1m3_command_ExitMaintenance"); m1m3SAL.salProcessor("m1m3_command_ExitMaintenance"); }
    if (enabledTopics[MESSAGE_TOPIC_Shutdown]) { m1m3SAL.salCommand("m1m3_command_Shutdown"); m1m3SAL.salProcessor("m1m3_command_Shutdown"); }


    do {
        // Initialization Done
        printf("Done\n");
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
            printf("SALPIPE: Problem with TCP_NODELAY\n");
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
        printf("Closing client connection\n");
        if (close(connectionHandle) < 0) {
            fprintf(stderr, "SALPIPE: Error calling close()\n");
            exit(EXIT_FAILURE);
        }
        
        if (stayAlive) {
            // Wait for a client to connect
            printf("Waiting for client\n");
            if ((connectionHandle = accept(listenerHandle, NULL, NULL) ) < 0 ) {
                fprintf(stderr, "SALPIPE: Error calling accept()\n");
                exit(EXIT_FAILURE);
            }
        }
    } while(stayAlive);

    // Close SAL
    printf("Shutting down SAL\n");
    m1m3SAL.salShutdown();
}
