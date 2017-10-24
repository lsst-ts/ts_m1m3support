/*
 * DataTypes.h
 *
 *  Created on: Sep 27, 2017
 *      Author: ccontaxis
 */

#ifndef DATATYPES_H_
#define DATATYPES_H_

#define ACK_INPROGRESS 301
#define ACK_COMPLETE   303
#define ACK_FAILED    -302

#define SUBNET_COUNT 5
#define FA_COUNT     156
#define HP_COUNT     6

typedef signed char        int8_t;
typedef short int          int16_t;
typedef int                int32_t;
typedef long int           int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int       uint32_t;
typedef unsigned long int  uint64_t;
typedef unsigned char      bool_t;

// SAL__CMD_ACK        300
// SAL__CMD_INPROGRESS 301
// SAL__CMD_STALLED    302
// SAL__CMD_COMPLETE   303
// SAL__CMD_NOPERM    -300
// SAL__CMD_NOACK     -301
// SAL__CMD_FAILED    -302
// SAL__CMD_ABORTED   -303
// SAL__CMD_TIMEOUT   -304

#endif /* DATATYPES_H_ */
