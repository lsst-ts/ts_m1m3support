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
#define MESSAGE_TOPIC_ENTER_CONTROL 1
#define MESSAGE_TOPIC_START 2
#define MESSAGE_TOPIC_ENABLE 3
#define MESSAGE_TOPIC_DISABLE 4
#define MESSAGE_TOPIC_STANDBY 5
#define MESSAGE_TOPIC_EXIT_CONTROL 6
#define MESSAGE_TOPIC_APPLIED_SETTINGS_MATCH_START 7
#define MESSAGE_TOPIC_ERROR_CODE 8
#define MESSAGE_TOPIC_SETTING_VERSIONS 9
#define MESSAGE_TOPIC_SUMMARY_STATE 10
#define TOPIC_COUNT 255

#define POLL_ERROR -1
#define POLL_TIMEOUT 0

#define HEADER_BUFFER_SIZE 6
#define DATA_BUFFER_SIZE 1024

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
m1m3_command_enterControlC commandEnterControlData;
m1m3_command_startC commandStartData;
m1m3_command_enableC commandEnableData;
m1m3_command_disableC commandDisableData;
m1m3_command_standbyC commandStandbyData;
m1m3_command_exitControlC commandExitControlData;

m1m3_logevent_AppliedSettingsMatchStartC eventAppliedSettingsMatchStartData;
m1m3_logevent_ErrorCodeC eventErrorCodeData;
m1m3_logevent_SettingVersionsC eventSettingVerionsData;
m1m3_logevent_SummaryStateC eventSummaryStateData;

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
    b[(*i)++] = (data >> 40) & 0xFF;
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
    b[(*i)++] = (data >> 40) & 0xFF;
    b[(*i)++] = (data >> 24) & 0xFF;
    b[(*i)++] = (data >> 16) & 0xFF;
    b[(*i)++] = (data >> 8) & 0xFF;
    b[(*i)++] = data & 0xFF;
}

bool_t readClientHeader() {
    if (read(connectionHandle, buffer, sizeof(MessageHeader)) == sizeof(MessageHeader)) {
        bufferIndex = 0;
        messageHeader.type = getU8(buffer, &bufferIndex);
        messageHeader.topic = getU8(buffer, &bufferIndex);
        messageHeader.size = getI32(buffer, &bufferIndex);
        printf("type %d topic %d size %d\n", messageHeader.type, messageHeader.topic, messageHeader.size);
        return true;
    }
    return false;
}

bool_t readClientData() {
    return read(connectionHandle, buffer, messageHeader.size) == messageHeader.size;
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
    headerIndex = 0;
    setU8(headerBuffer, &headerIndex, type);
    setU8(headerBuffer, &headerIndex, topic);
    setI32(headerBuffer, &headerIndex, size);
    return writeBuffer(headerBuffer, sizeof(MessageHeader));
}

void processPutSampleClientRequest() {
    printf("PUT_SAMPLE ");
    switch(messageHeader.topic) {
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

void processLogEventAppliedSettingsMatchStartClientRequest() {
    printf("AppliedSettingsMatchStart\n");
    if (enabledTopics[MESSAGE_TOPIC_APPLIED_SETTINGS_MATCH_START]) {
        bufferIndex = 0;
        eventAppliedSettingsMatchStartData.appliedSettingsMatchStartIsTrue = getU8(buffer, &bufferIndex);
        eventAppliedSettingsMatchStartData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_AppliedSettingsMatchStart(&eventAppliedSettingsMatchStartData, eventAppliedSettingsMatchStartData.priority);
    }
}

void processLogEventErrorCodeClientRequest() {
    printf("ErrorCode\n");
    if (enabledTopics[MESSAGE_TOPIC_ERROR_CODE]) {
        bufferIndex = 0;
        eventErrorCodeData.errorCode = getI32(buffer, &bufferIndex);
        eventErrorCodeData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_ErrorCode(&eventErrorCodeData, eventErrorCodeData.priority);
    }
}

void processLogEventSettingVersionsClientRequest() {
    printf("SettingVersions\n");
    if (enabledTopics[MESSAGE_TOPIC_SETTING_VERSIONS]) {
        bufferIndex = 0;
        eventSettingVerionsData.recommendedSettingVersion = "";
        eventSettingVerionsData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_SettingVersions(&eventSettingVerionsData, eventSettingVerionsData.priority);
    }
}

void processLogEventSummaryStateClientRequest() {
    printf("SummaryState\n");
    if (enabledTopics[MESSAGE_TOPIC_SUMMARY_STATE]) {
        bufferIndex = 0;
        eventSummaryStateData.SummaryStateValue = getI32(buffer, &bufferIndex);
        eventSummaryStateData.priority = getI32(buffer, &bufferIndex);
        m1m3SAL.logEvent_SummaryState(&eventSummaryStateData, eventSummaryStateData.priority);
    }
}

void processLogEventClientRequest() {
    printf("LOG_EVENT ");
    switch(messageHeader.topic) {
        case MESSAGE_TOPIC_APPLIED_SETTINGS_MATCH_START: processLogEventAppliedSettingsMatchStartClientRequest(); break;
        case MESSAGE_TOPIC_ERROR_CODE: processLogEventErrorCodeClientRequest(); break;
        case MESSAGE_TOPIC_SETTING_VERSIONS: processLogEventSettingVersionsClientRequest(); break;
        case MESSAGE_TOPIC_SUMMARY_STATE: processLogEventSummaryStateClientRequest(); break;
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

void processIssueCommandEnterControlClientRequest() {
    printf("EnterControl\n");
    if (enabledTopics[MESSAGE_TOPIC_ENTER_CONTROL]) {
        bufferIndex = 0;
        commandEnterControlData.state = getU8(buffer, &bufferIndex);
        commandId = m1m3SAL.issueCommand_enterControl(&commandEnterControlData);
    }
}

void processIssueCommandStartClientRequest() {
    printf("Start\n");
    if (enabledTopics[MESSAGE_TOPIC_START]) {
        bufferIndex = 0;
        commandStartData.configuration = "1";
        commandId = m1m3SAL.issueCommand_start(&commandStartData);
    }
}

void processIssueCommandEnableClientRequest() {
    printf("Enable\n");
    if (enabledTopics[MESSAGE_TOPIC_ENABLE]) {
        bufferIndex = 0;
        commandEnableData.state = getU8(buffer, &bufferIndex);
        commandId = m1m3SAL.issueCommand_enable(&commandEnableData);
    }
}

void processIssueCommandDisableClientRequest() {
    printf("Disable\n");
    if (enabledTopics[MESSAGE_TOPIC_DISABLE]) {
        bufferIndex = 0;
        commandDisableData.state = getU8(buffer, &bufferIndex);
        commandId = m1m3SAL.issueCommand_disable(&commandDisableData); 
    }
}

void processIssueCommandStandbyClientRequest() {
    printf("Standby\n");
    if (enabledTopics[MESSAGE_TOPIC_STANDBY]) {
        bufferIndex = 0;
        commandStandbyData.state = getU8(buffer, &bufferIndex);
        commandId = m1m3SAL.issueCommand_standby(&commandStandbyData);
    }
}

void processIssueCommandExitControlClientRequest() {
    printf("ExitControl\n");
    if (enabledTopics[MESSAGE_TOPIC_EXIT_CONTROL]) {
        bufferIndex = 0;
        commandExitControlData.state = getU8(buffer, &bufferIndex);
        commandId = m1m3SAL.issueCommand_exitControl(&commandExitControlData);
    }
}

void processIssueCommandClientRequest() {
    printf("ISSUE_COMMAND ");
    switch(messageHeader.topic) {
        case MESSAGE_TOPIC_ENTER_CONTROL: processIssueCommandEnterControlClientRequest(); break;
        case MESSAGE_TOPIC_START: processIssueCommandStartClientRequest(); break;
        case MESSAGE_TOPIC_ENABLE: processIssueCommandEnableClientRequest(); break;
        case MESSAGE_TOPIC_DISABLE: processIssueCommandDisableClientRequest(); break;
        case MESSAGE_TOPIC_STANDBY: processIssueCommandStandbyClientRequest(); break;
        case MESSAGE_TOPIC_EXIT_CONTROL: processIssueCommandExitControlClientRequest(); break;
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

void processGetEventAppliedSettingsMatchStartSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_APPLIED_SETTINGS_MATCH_START]) {
        rc = m1m3SAL.getEvent_AppliedSettingsMatchStart(&eventAppliedSettingsMatchStartData);
        if (rc == SAL__OK) {
            printf("GET_EVENT AppliedSettingsMatchStart\n");
            bufferIndex = 0;
            setU8(buffer, &bufferIndex, eventAppliedSettingsMatchStartData.appliedSettingsMatchStartIsTrue);
            setI32(buffer, &bufferIndex, eventAppliedSettingsMatchStartData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_APPLIED_SETTINGS_MATCH_START, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventErrorCodeSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ERROR_CODE]) {
        rc = m1m3SAL.getEvent_ErrorCode(&eventErrorCodeData);
        if (rc == SAL__OK) {
            printf("GET_EVENT ErrorCode\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, eventErrorCodeData.errorCode);
            setI32(buffer, &bufferIndex, eventErrorCodeData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_ERROR_CODE, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventSettingVerionsSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_SETTING_VERSIONS]) {
        rc = m1m3SAL.getEvent_SettingVersions(&eventSettingVerionsData);
        if (rc == SAL__OK) {
            printf("GET_EVENT SettingVersions\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, eventSettingVerionsData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_SETTING_VERSIONS, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventSummaryStateSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_SUMMARY_STATE]) {
        rc = m1m3SAL.getEvent_SummaryState(&eventSummaryStateData);
        if (rc == SAL__OK) {
            printf("GET_EVENT SummaryState\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, eventSummaryStateData.SummaryStateValue);
            setI32(buffer, &bufferIndex, eventSummaryStateData.priority);
            writeClientHeader(MESSAGE_TYPE_GET_EVENT, MESSAGE_TOPIC_SUMMARY_STATE, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processGetEventSALRequest() {
    processGetEventAppliedSettingsMatchStartSALRequest();
    processGetEventErrorCodeSALRequest();
    processGetEventSettingVerionsSALRequest();
    processGetEventSummaryStateSALRequest();
}

void processAcceptCommandEnterControlSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ENTER_CONTROL]) {
        commandId = m1m3SAL.acceptCommand_enterControl(&commandEnterControlData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND EnterControl\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
            setU8(buffer, &bufferIndex, commandEnterControlData.state);
            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_ENTER_CONTROL, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandStartSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_START]) {
        commandId = m1m3SAL.acceptCommand_start(&commandStartData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND Start\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_START, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandEnableSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_ENABLE]) {
        commandId = m1m3SAL.acceptCommand_enable(&commandEnableData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND Enable\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
            setU8(buffer, &bufferIndex, commandEnableData.state);
            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_ENABLE, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandDisableSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_DISABLE]) {
        commandId = m1m3SAL.acceptCommand_disable(&commandDisableData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND Disable\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
            setU8(buffer, &bufferIndex, commandDisableData.state);
            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_DISABLE, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandStandbySALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_STANDBY]) {
        commandId = m1m3SAL.acceptCommand_standby(&commandStandbyData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND Standby\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
            setU8(buffer, &bufferIndex, commandStandbyData.state);
            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_STANDBY, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandExitControlSALRequest() {
    if (enabledTopics[MESSAGE_TOPIC_EXIT_CONTROL]) {
        commandId = m1m3SAL.acceptCommand_exitControl(&commandExitControlData);
        if (commandId > 0) {
            printf("ACCEPT_COMMAND ExitControl\n");
            bufferIndex = 0;
            setI32(buffer, &bufferIndex, commandId);
            setU8(buffer, &bufferIndex, commandExitControlData.state);
            writeClientHeader(MESSAGE_TYPE_ACCEPT_COMMAND, MESSAGE_TOPIC_EXIT_CONTROL, bufferIndex);
            writeBuffer(buffer, bufferIndex);
        }
    }
}

void processAcceptCommandSALRequest() {
    processAcceptCommandEnterControlSALRequest();
    processAcceptCommandStartSALRequest();
    processAcceptCommandEnableSALRequest();
    processAcceptCommandDisableSALRequest();
    processAcceptCommandStandbySALRequest();
    processAcceptCommandExitControlSALRequest();
}

void processSALRequest() {
    processGetEventSALRequest();
    processAcceptCommandSALRequest();
}

int main(int argumentCount, char *argumentValues[]) {
    memset(&listenerAddress, 0, sizeof(sockaddr_in));
	memset(&messageHeader, 0, sizeof(MessageHeader));

    // Process arguments for port number
    if ( argumentCount >= 3 ) {
        port = strtol(argumentValues[1], &endptr, 0);
        if ( *endptr ) {
            fprintf(stderr, "ECHOSERV: Invalid port number.\n");
            exit(EXIT_FAILURE);
        }
        
        int enabledTopic = 0;
        for(int i = 2; i < argumentCount; i++) {
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
        fprintf(stderr, "ECHOSERV: Invalid arguments.\n");
        exit(EXIT_FAILURE);
    }

    // Create listener handle
    if ((listenerHandle = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error creating listening socket.\n");
        exit(EXIT_FAILURE);
    }

    // Create listener address
    listenerAddress.sin_family      = AF_INET;
    listenerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    listenerAddress.sin_port        = htons(port);

    // Bind listener
    if ( bind(listenerHandle, (struct sockaddr *) &listenerAddress, sizeof(listenerAddress)) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling bind()\n");
        exit(EXIT_FAILURE);
    }
    
    // Start listener
    if ( listen(listenerHandle, LISTENQ) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling listen()\n");
        exit(EXIT_FAILURE);
    }
    
    // Wait for a client to connect
    printf("Waiting for client\n");
    if ((connectionHandle = accept(listenerHandle, NULL, NULL) ) < 0 ) {
        fprintf(stderr, "ECHOSERV: Error calling accept()\n");
        exit(EXIT_FAILURE);
    }

    // Initialization Start
    printf("Initializing ");
    
    // Initialize Commands
    printf("Commands ");
    if (enabledTopics[MESSAGE_TOPIC_ENTER_CONTROL]) {
        m1m3SAL.salCommand("m1m3_command_enterControl");
        m1m3SAL.salProcessor("m1m3_command_enterControl");
    }
    if (enabledTopics[MESSAGE_TOPIC_START]) {
        m1m3SAL.salCommand("m1m3_command_start");
        m1m3SAL.salProcessor("m1m3_command_start");
    }
    if (enabledTopics[MESSAGE_TOPIC_ENABLE]) {
        m1m3SAL.salCommand("m1m3_command_enable");
        m1m3SAL.salProcessor("m1m3_command_enable");
    }
    if (enabledTopics[MESSAGE_TOPIC_DISABLE]) {
        m1m3SAL.salCommand("m1m3_command_disable");
        m1m3SAL.salProcessor("m1m3_command_disable");
    }
    if (enabledTopics[MESSAGE_TOPIC_STANDBY]) {
        m1m3SAL.salCommand("m1m3_command_standby");
        m1m3SAL.salProcessor("m1m3_command_standby");
    }
    if (enabledTopics[MESSAGE_TOPIC_EXIT_CONTROL]) {
        m1m3SAL.salCommand("m1m3_command_exitControl");
        m1m3SAL.salProcessor("m1m3_command_exitControl");
    }
    
    // Initialize Events
    printf("Events ");
    if (enabledTopics[MESSAGE_TOPIC_APPLIED_SETTINGS_MATCH_START]) {
        m1m3SAL.salEvent("m1m3_logevent_AppliedSettingsMatchStart");
    }
    if (enabledTopics[MESSAGE_TOPIC_ERROR_CODE]) {
        m1m3SAL.salEvent("m1m3_logevent_ErrorCode");
    }
    if (enabledTopics[MESSAGE_TOPIC_SETTING_VERSIONS]) {
        m1m3SAL.salEvent("m1m3_logevent_SettingVersions");
    }
    if (enabledTopics[MESSAGE_TOPIC_SUMMARY_STATE]) {
        m1m3SAL.salEvent("m1m3_logevent_SummaryState");
    }
    
    // Initialize Telemetry
    printf("Telemetry ");
    
    // Initialization Done
    printf("Done\n");
    headerBuffer[0] = 0xFF;
    writeBuffer(headerBuffer, 1);
    
    // Setup polling
    memset(&pollHandle, 0, sizeof(pollfd));
    pollHandle.fd = connectionHandle;
    pollHandle.events = POLLIN;
    
    int flag = 1;
    rc = setsockopt(connectionHandle, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(int));
    if (rc < 0) {
        printf("Problem with TCP_NODELAY\n");
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
        fprintf(stderr, "Error calling close()\n");
        exit(EXIT_FAILURE);
    }

    // Close SAL
    printf("Shutting down SAL\n");
    m1m3SAL.salShutdown();
}