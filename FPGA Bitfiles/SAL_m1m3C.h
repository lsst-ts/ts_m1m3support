
#ifndef _SAL_m1m3C_
#define _SAL_m1m3C_

#include <string>

using namespace std;

struct m1m3_AirDataC
{
  double Timestamp;
  float CellAirPressure;
  float HardpointAirPressure;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_AirDataC()  { std::cout << "m1m3_AirDataC()"  << std::endl; }
  ~m1m3_AirDataC() { std::cout << "~m1m3_AirDataC()"  << std::endl; }
#endif
};

struct m1m3_DynamicDataC
{
  double Timestamp;
  double AzimuthVelocity;
  double ElevationVelocity;
  double AzimuthAcceleration;
  double ElevationAcceleration;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_DynamicDataC()  { std::cout << "m1m3_DynamicDataC()"  << std::endl; }
  ~m1m3_DynamicDataC() { std::cout << "~m1m3_DynamicDataC()"  << std::endl; }
#endif
};

struct m1m3_ElevationDataC
{
  double Timestamp;
  double InclinometerElevation;
  double MeasuredElevation;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_ElevationDataC()  { std::cout << "m1m3_ElevationDataC()"  << std::endl; }
  ~m1m3_ElevationDataC() { std::cout << "~m1m3_ElevationDataC()"  << std::endl; }
#endif
};

struct m1m3_FPGADataC
{
  double Timestamp;
  int ModbusSubnetATxInternalFIFOOverflow;
  int ModbusSubnetAInvalidInstruction;
  int ModbusSubnetAWaitForRxFrameTimeout;
  int ModbusSubnetAStartBitError;
  int ModbusSubnetAStopBitError;
  int ModbusSubnetARxDataFIFOOverflow;
 unsigned int ModbusSubnetATxByteCount;
 unsigned int ModbusSubnetATxFrameCount;
 unsigned int ModbusSubnetARxByteCount;
 unsigned int ModbusSubnetARxFrameCount;
  int ModbusSubnetBTxInternalFIFOOverflow;
  int ModbusSubnetBInvalidInstruction;
  int ModbusSubnetBWaitForRxFrameTimeout;
  int ModbusSubnetBStartBitError;
  int ModbusSubnetBStopBitError;
  int ModbusSubnetBRxDataFIFOOverflow;
 unsigned int ModbusSubnetBTxByteCount;
 unsigned int ModbusSubnetBTxFrameCount;
 unsigned int ModbusSubnetBRxByteCount;
 unsigned int ModbusSubnetBRxFrameCount;
  int ModbusSubnetCTxInternalFIFOOverflow;
  int ModbusSubnetCInvalidInstruction;
  int ModbusSubnetCWaitForRxFrameTimeout;
  int ModbusSubnetCStartBitError;
  int ModbusSubnetCStopBitError;
  int ModbusSubnetCRxDataFIFOOverflow;
 unsigned int ModbusSubnetCTxByteCount;
 unsigned int ModbusSubnetCTxFrameCount;
 unsigned int ModbusSubnetCRxByteCount;
 unsigned int ModbusSubnetCRxFrameCount;
  int ModbusSubnetDTxInternalFIFOOverflow;
  int ModbusSubnetDInvalidInstruction;
  int ModbusSubnetDWaitForRxFrameTimeout;
  int ModbusSubnetDStartBitError;
  int ModbusSubnetDStopBitError;
  int ModbusSubnetDRxDataFIFOOverflow;
 unsigned int ModbusSubnetDTxByteCount;
 unsigned int ModbusSubnetDTxFrameCount;
 unsigned int ModbusSubnetDRxByteCount;
 unsigned int ModbusSubnetDRxFrameCount;
  int ModbusSubnetETxInternalFIFOOverflow;
  int ModbusSubnetEInvalidInstruction;
  int ModbusSubnetEWaitForRxFrameTimeout;
  int ModbusSubnetEStartBitError;
  int ModbusSubnetEStopBitError;
  int ModbusSubnetERxDataFIFOOverflow;
 unsigned int ModbusSubnetETxByteCount;
 unsigned int ModbusSubnetETxFrameCount;
 unsigned int ModbusSubnetERxByteCount;
 unsigned int ModbusSubnetERxFrameCount;
  double FPGATime;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_FPGADataC()  { std::cout << "m1m3_FPGADataC()"  << std::endl; }
  ~m1m3_FPGADataC() { std::cout << "~m1m3_FPGADataC()"  << std::endl; }
#endif
};

struct m1m3_ForceActuatorDataC
{
  double Timestamp;
   int SlewFlagCommanded[156];
   double XSetpointCommanded[156];
   double YSetpointCommanded[156];
   double ZSetpointCommanded[156];
   float XForce[156];
   float YForce[156];
   float ZForce[156];
   int ILCFault[156];
   int DCAFault[156];
   unsigned char BroadcastCounter[156];

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_ForceActuatorDataC()  { std::cout << "m1m3_ForceActuatorDataC()"  << std::endl; }
  ~m1m3_ForceActuatorDataC() { std::cout << "~m1m3_ForceActuatorDataC()"  << std::endl; }
#endif
};

struct m1m3_ForceActuatorStatusC
{
  double Timestamp;
   unsigned char Mode[156];
   int MajorFault[156];
   int MinorFault[156];
   int FaultOverridden[156];
   int DCAFault[156];
   int WatchdogReset[156];
   int BrownoutDetected[156];
   int EventTrapReset[156];
   int MotorPowerFault[156];
   int SSRPowerFault[156];
   int AUXPowerFault[156];
   int SMCPowerFault[156];
   int DCAOutputsEnabled[156];
   int DCAPowerFault[156];
   int DCAAmplifierAFault[156];
   int DCAAmplifierBFault[156];
   int DCAEventTrapReset[156];

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_ForceActuatorStatusC()  { std::cout << "m1m3_ForceActuatorStatusC()"  << std::endl; }
  ~m1m3_ForceActuatorStatusC() { std::cout << "~m1m3_ForceActuatorStatusC()"  << std::endl; }
#endif
};

struct m1m3_HardpointDataC
{
  double Timestamp;
   short StepsCommanded[6];
   float Force[6];
   int Encoder[6];
   int ILCFault[6];
   int CWLimitOperated[6];
   int CCWLimitOperated[6];
   unsigned char BroadcastCounter[6];

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_HardpointDataC()  { std::cout << "m1m3_HardpointDataC()"  << std::endl; }
  ~m1m3_HardpointDataC() { std::cout << "~m1m3_HardpointDataC()"  << std::endl; }
#endif
};

struct m1m3_HardpointStatusC
{
  double Timestamp;
   unsigned char Mode[6];
   int MajorFault[6];
   int MinorFault[6];
   int FaultOverridden[6];
   int WatchdogReset[6];
   int BrownoutDetected[6];
   int EventTrapReset[6];
   int MotorPowerFault[6];
   int SSRPowerFault[6];
   int AUXPowerFault[6];
   int SMCPowerFault[6];

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_HardpointStatusC()  { std::cout << "m1m3_HardpointStatusC()"  << std::endl; }
  ~m1m3_HardpointStatusC() { std::cout << "~m1m3_HardpointStatusC()"  << std::endl; }
#endif
};

struct m1m3_IMSDataC
{
  double Timestamp;
  double XPosition;
  double YPosition;
  double ZPosition;
  double XRotation;
  double YRotation;
  double ZRotation;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_IMSDataC()  { std::cout << "m1m3_IMSDataC()"  << std::endl; }
  ~m1m3_IMSDataC() { std::cout << "~m1m3_IMSDataC()"  << std::endl; }
#endif
};

struct m1m3_MirrorForceDataC
{
  double Timestamp;
  double Fx;
  double Fy;
  double Fz;
  double Mx;
  double My;
  double Mz;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_MirrorForceDataC()  { std::cout << "m1m3_MirrorForceDataC()"  << std::endl; }
  ~m1m3_MirrorForceDataC() { std::cout << "~m1m3_MirrorForceDataC()"  << std::endl; }
#endif
};

struct m1m3_command_AbortLowerM1M3C
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int AbortLowerM1M3;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_AbortLowerM1M3C()  { std::cout << "m1m3_command_AbortLowerM1M3C()"  << std::endl; }
  ~m1m3_command_AbortLowerM1M3C() { std::cout << "~m1m3_command_AbortLowerM1M3C()"  << std::endl; }
#endif
};

struct m1m3_command_AbortRaiseM1M3C
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int AbortRaiseM1M3;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_AbortRaiseM1M3C()  { std::cout << "m1m3_command_AbortRaiseM1M3C()"  << std::endl; }
  ~m1m3_command_AbortRaiseM1M3C() { std::cout << "~m1m3_command_AbortRaiseM1M3C()"  << std::endl; }
#endif
};

struct m1m3_command_ApplyAOSCorrectionByBendingModesC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
   double Coefficients[22];

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_ApplyAOSCorrectionByBendingModesC()  { std::cout << "m1m3_command_ApplyAOSCorrectionByBendingModesC()"  << std::endl; }
  ~m1m3_command_ApplyAOSCorrectionByBendingModesC() { std::cout << "~m1m3_command_ApplyAOSCorrectionByBendingModesC()"  << std::endl; }
#endif
};

struct m1m3_command_ApplyAOSCorrectionByForcesC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
   double ZForces[156];

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_ApplyAOSCorrectionByForcesC()  { std::cout << "m1m3_command_ApplyAOSCorrectionByForcesC()"  << std::endl; }
  ~m1m3_command_ApplyAOSCorrectionByForcesC() { std::cout << "~m1m3_command_ApplyAOSCorrectionByForcesC()"  << std::endl; }
#endif
};

struct m1m3_command_ApplyAberrationByBendingModesC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
   double Coefficients[22];

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_ApplyAberrationByBendingModesC()  { std::cout << "m1m3_command_ApplyAberrationByBendingModesC()"  << std::endl; }
  ~m1m3_command_ApplyAberrationByBendingModesC() { std::cout << "~m1m3_command_ApplyAberrationByBendingModesC()"  << std::endl; }
#endif
};

struct m1m3_command_ApplyAberrationByForcesC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
   double ZForces[156];

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_ApplyAberrationByForcesC()  { std::cout << "m1m3_command_ApplyAberrationByForcesC()"  << std::endl; }
  ~m1m3_command_ApplyAberrationByForcesC() { std::cout << "~m1m3_command_ApplyAberrationByForcesC()"  << std::endl; }
#endif
};

struct m1m3_command_ClearAOSCorrectionC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int ClearAOSCorrection;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_ClearAOSCorrectionC()  { std::cout << "m1m3_command_ClearAOSCorrectionC()"  << std::endl; }
  ~m1m3_command_ClearAOSCorrectionC() { std::cout << "~m1m3_command_ClearAOSCorrectionC()"  << std::endl; }
#endif
};

struct m1m3_command_ClearAberrationC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int ClearAberration;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_ClearAberrationC()  { std::cout << "m1m3_command_ClearAberrationC()"  << std::endl; }
  ~m1m3_command_ClearAberrationC() { std::cout << "~m1m3_command_ClearAberrationC()"  << std::endl; }
#endif
};

struct m1m3_command_DisableC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int Disable;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_DisableC()  { std::cout << "m1m3_command_DisableC()"  << std::endl; }
  ~m1m3_command_DisableC() { std::cout << "~m1m3_command_DisableC()"  << std::endl; }
#endif
};

struct m1m3_command_EnableC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int Enable;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_EnableC()  { std::cout << "m1m3_command_EnableC()"  << std::endl; }
  ~m1m3_command_EnableC() { std::cout << "~m1m3_command_EnableC()"  << std::endl; }
#endif
};

struct m1m3_command_EnterMaintenanceC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int EnterMaintenance;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_EnterMaintenanceC()  { std::cout << "m1m3_command_EnterMaintenanceC()"  << std::endl; }
  ~m1m3_command_EnterMaintenanceC() { std::cout << "~m1m3_command_EnterMaintenanceC()"  << std::endl; }
#endif
};

struct m1m3_command_ExitC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int Exit;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_ExitC()  { std::cout << "m1m3_command_ExitC()"  << std::endl; }
  ~m1m3_command_ExitC() { std::cout << "~m1m3_command_ExitC()"  << std::endl; }
#endif
};

struct m1m3_command_ExitMaintenanceC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int ExitMaintenance;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_ExitMaintenanceC()  { std::cout << "m1m3_command_ExitMaintenanceC()"  << std::endl; }
  ~m1m3_command_ExitMaintenanceC() { std::cout << "~m1m3_command_ExitMaintenanceC()"  << std::endl; }
#endif
};

struct m1m3_command_LowerM1M3C
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int LowerM1M3;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_LowerM1M3C()  { std::cout << "m1m3_command_LowerM1M3C()"  << std::endl; }
  ~m1m3_command_LowerM1M3C() { std::cout << "~m1m3_command_LowerM1M3C()"  << std::endl; }
#endif
};

struct m1m3_command_ManipulateM1M3C
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  double XTranslation;
  double YTranslation;
  double ZTranslation;
  double XRotation;
  double YRotation;
  double ZRotation;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_ManipulateM1M3C()  { std::cout << "m1m3_command_ManipulateM1M3C()"  << std::endl; }
  ~m1m3_command_ManipulateM1M3C() { std::cout << "~m1m3_command_ManipulateM1M3C()"  << std::endl; }
#endif
};

struct m1m3_command_MoveHardpointActuatorsC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
   unsigned char Steps[6];

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_MoveHardpointActuatorsC()  { std::cout << "m1m3_command_MoveHardpointActuatorsC()"  << std::endl; }
  ~m1m3_command_MoveHardpointActuatorsC() { std::cout << "~m1m3_command_MoveHardpointActuatorsC()"  << std::endl; }
#endif
};

struct m1m3_command_RaiseM1M3C
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int RaiseM1M3;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_RaiseM1M3C()  { std::cout << "m1m3_command_RaiseM1M3C()"  << std::endl; }
  ~m1m3_command_RaiseM1M3C() { std::cout << "~m1m3_command_RaiseM1M3C()"  << std::endl; }
#endif
};

struct m1m3_command_ShutdownC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int Shutdown;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_ShutdownC()  { std::cout << "m1m3_command_ShutdownC()"  << std::endl; }
  ~m1m3_command_ShutdownC() { std::cout << "~m1m3_command_ShutdownC()"  << std::endl; }
#endif
};

struct m1m3_command_StandbyC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int Standby;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_StandbyC()  { std::cout << "m1m3_command_StandbyC()"  << std::endl; }
  ~m1m3_command_StandbyC() { std::cout << "~m1m3_command_StandbyC()"  << std::endl; }
#endif
};

struct m1m3_command_StartC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int Start;
  std::string	SettingsToApply;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_StartC()  { std::cout << "m1m3_command_StartC()"  << std::endl; }
  ~m1m3_command_StartC() { std::cout << "~m1m3_command_StartC()"  << std::endl; }
#endif
};

struct m1m3_command_TestAirC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int TestAir;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_TestAirC()  { std::cout << "m1m3_command_TestAirC()"  << std::endl; }
  ~m1m3_command_TestAirC() { std::cout << "~m1m3_command_TestAirC()"  << std::endl; }
#endif
};

struct m1m3_command_TestForceActuatorC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int ActuatorId;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_TestForceActuatorC()  { std::cout << "m1m3_command_TestForceActuatorC()"  << std::endl; }
  ~m1m3_command_TestForceActuatorC() { std::cout << "~m1m3_command_TestForceActuatorC()"  << std::endl; }
#endif
};

struct m1m3_command_TestHardpointC
{
  std::string	device;
  std::string	property;
  std::string	action;
  std::string	value;
  int ActuatorId;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_command_TestHardpointC()  { std::cout << "m1m3_command_TestHardpointC()"  << std::endl; }
  ~m1m3_command_TestHardpointC() { std::cout << "~m1m3_command_TestHardpointC()"  << std::endl; }
#endif
};

struct m1m3_logevent_ActuatorBroadcastCounterC
{
  double Timestamp;
  int ActuatorId;
  int CounterOk;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_ActuatorBroadcastCounterC()  { std::cout << "m1m3_logevent_ActuatorBroadcastCounterC()"  << std::endl; }
  ~m1m3_logevent_ActuatorBroadcastCounterC() { std::cout << "~m1m3_logevent_ActuatorBroadcastCounterC()"  << std::endl; }
#endif
};

struct m1m3_logevent_ActuatorTestStatusC
{
  double Timestamp;
  int AllTestsRan;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_ActuatorTestStatusC()  { std::cout << "m1m3_logevent_ActuatorTestStatusC()"  << std::endl; }
  ~m1m3_logevent_ActuatorTestStatusC() { std::cout << "~m1m3_logevent_ActuatorTestStatusC()"  << std::endl; }
#endif
};

struct m1m3_logevent_AirStatusC
{
  double Timestamp;
  int AirCommandedOn;
  int AirOn;
  int AirPressureOk;
  int HardpointAirPressureOk;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_AirStatusC()  { std::cout << "m1m3_logevent_AirStatusC()"  << std::endl; }
  ~m1m3_logevent_AirStatusC() { std::cout << "~m1m3_logevent_AirStatusC()"  << std::endl; }
#endif
};

struct m1m3_logevent_AppliedSettingsMatchStartC
{
  double Timestamp;
  int AppliedSettingsMatchStart;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_AppliedSettingsMatchStartC()  { std::cout << "m1m3_logevent_AppliedSettingsMatchStartC()"  << std::endl; }
  ~m1m3_logevent_AppliedSettingsMatchStartC() { std::cout << "~m1m3_logevent_AppliedSettingsMatchStartC()"  << std::endl; }
#endif
};

struct m1m3_logevent_CellChecksC
{
  double Timestamp;
  int MirrorDoorClosed;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_CellChecksC()  { std::cout << "m1m3_logevent_CellChecksC()"  << std::endl; }
  ~m1m3_logevent_CellChecksC() { std::cout << "~m1m3_logevent_CellChecksC()"  << std::endl; }
#endif
};

struct m1m3_logevent_DetailedStateC
{
  double Timestamp;
  int DetailedState;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_DetailedStateC()  { std::cout << "m1m3_logevent_DetailedStateC()"  << std::endl; }
  ~m1m3_logevent_DetailedStateC() { std::cout << "~m1m3_logevent_DetailedStateC()"  << std::endl; }
#endif
};

struct m1m3_logevent_ElevationAngleChecksC
{
  double Timestamp;
  int MatchesTMA;
  int MatchesInclinometer;
  int MatchesMeasured;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_ElevationAngleChecksC()  { std::cout << "m1m3_logevent_ElevationAngleChecksC()"  << std::endl; }
  ~m1m3_logevent_ElevationAngleChecksC() { std::cout << "~m1m3_logevent_ElevationAngleChecksC()"  << std::endl; }
#endif
};

struct m1m3_logevent_ErrorCodeC
{
  double Timestamp;
  int ErrorCode;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_ErrorCodeC()  { std::cout << "m1m3_logevent_ErrorCodeC()"  << std::endl; }
  ~m1m3_logevent_ErrorCodeC() { std::cout << "~m1m3_logevent_ErrorCodeC()"  << std::endl; }
#endif
};

struct m1m3_logevent_ForceActuatorInfoC
{
  double Timestamp;
   int ReferenceId[156];
   float XPosition[156];
   float YPosition[156];
   float ZPosition[156];
   long long ILCUniqueId[156];
   unsigned char ILCApplicationType[156];
   unsigned char NetworkNodeType[156];
   unsigned char ILCSelectedOptions[156];
   unsigned char NetworkNodeOptions[156];
   unsigned char MajorRevision[156];
   unsigned char MinorRevision[156];
   unsigned char ADCScanRate[156];
   float MainADCCalibrationK1[156];
   float MainADCCalibrationK2[156];
   float MainADCCalibrationK3[156];
   float MainADCCalibrationK4[156];
   float MainAxialLoadCellOffset[156];
   float MainLateralLoadCellOffset[156];
   float MainAxialLoadCellSensitivity[156];
   float MainLateralLoadCellSensitivity[156];
   float BackupADCCalibrationK1[156];
   float BackupADCCalibrationK2[156];
   float BackupADCCalibrationK3[156];
   float BackupADCCalibrationK4[156];
   float BackupAxialLoadCellOffset[156];
   float BackupLateralLoadCellOffset[156];
   float BackupAxialLoadCellSensitivity[156];
   float BackupLateralLoadCellSensitivity[156];
   float DCAAxialGain[156];
   float DCALateralGain[156];
   long long DCAUniqueId[156];
   unsigned char DCAFirmwareType[156];
   unsigned char DCAMajorRevision[156];
   unsigned char DCAMinorRevision[156];
   int MainCalibrationError[156];
   int BackupCalibrationError[156];
   int DCAFirmwareUpdate[156];
   int UniqueIdCRCError[156];
   int ApplicationTypeMismatch[156];
   int ApplicationMissing[156];
   int ApplicationCRCMismatch[156];
   int OneWireMissing[156];
   int OneWire1Mismatch[156];
   int OneWire2Mismatch[156];
   int DCAUniqueIdCRCError[156];
   int DCAMainCalibrationError[156];
   int DCABackupCalibrationError[156];
   int DCAApplicationMissing[156];
   int DCAApplicationCRCMismatch[156];
   int DCABootloaderActive[156];
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_ForceActuatorInfoC()  { std::cout << "m1m3_logevent_ForceActuatorInfoC()"  << std::endl; }
  ~m1m3_logevent_ForceActuatorInfoC() { std::cout << "~m1m3_logevent_ForceActuatorInfoC()"  << std::endl; }
#endif
};

struct m1m3_logevent_ForceActuatorSafetyChecksC
{
  double Timestamp;
  int AllInSafetyLimit;
   int SafetyLimitOk[156];
  int MirrorWeightOk;
  int XMomentOk;
  int YMomentOk;
  int AllNeighborsOk;
   int NeighborsOk[156];
  int MagnitudeOK;
  int AllGlobalBendingOk;
   int GlobalBendingOk[156];
  int FollowingErrorOk;
   int AxialFollowingErrorOk[156];
   int LateralFollowingErrorOk[156];
  int AOSNetForceOk;
  int AllElevationForcesOk;
   int ElevationForcesOk[156];
  int AllAzimuthForcesOk;
   int AzimuthForcesOk[156];
  int AllThermalForcesOk;
   int ThermalForcesOk[156];
  int AllAOSForcesOk;
   int AOSForcesOk[156];
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_ForceActuatorSafetyChecksC()  { std::cout << "m1m3_logevent_ForceActuatorSafetyChecksC()"  << std::endl; }
  ~m1m3_logevent_ForceActuatorSafetyChecksC() { std::cout << "~m1m3_logevent_ForceActuatorSafetyChecksC()"  << std::endl; }
#endif
};

struct m1m3_logevent_ForceActuatorTestC
{
  double Timestamp;
  int ActuatorId;
  int TestStatus;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_ForceActuatorTestC()  { std::cout << "m1m3_logevent_ForceActuatorTestC()"  << std::endl; }
  ~m1m3_logevent_ForceActuatorTestC() { std::cout << "~m1m3_logevent_ForceActuatorTestC()"  << std::endl; }
#endif
};

struct m1m3_logevent_HardpointActuatorBreakawayC
{
  double Timestamp;
  int ActuatorId;
  int ForceOk;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_HardpointActuatorBreakawayC()  { std::cout << "m1m3_logevent_HardpointActuatorBreakawayC()"  << std::endl; }
  ~m1m3_logevent_HardpointActuatorBreakawayC() { std::cout << "~m1m3_logevent_HardpointActuatorBreakawayC()"  << std::endl; }
#endif
};

struct m1m3_logevent_HardpointActuatorChaseC
{
  double Timestamp;
  int ActuatorId;
  int Chasing;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_HardpointActuatorChaseC()  { std::cout << "m1m3_logevent_HardpointActuatorChaseC()"  << std::endl; }
  ~m1m3_logevent_HardpointActuatorChaseC() { std::cout << "~m1m3_logevent_HardpointActuatorChaseC()"  << std::endl; }
#endif
};

struct m1m3_logevent_HardpointActuatorInfoC
{
  double Timestamp;
   int ReferenceId[6];
   int ReferencePosition[6];
   float XPosition[6];
   float YPosition[6];
   float ZPosition[6];
   long long ILCUniqueId[6];
   unsigned char ILCApplicationType[6];
   unsigned char NetworkNodeType[6];
   unsigned char ILCSelectedOptions[6];
   unsigned char NetworkNodeOptions[6];
   unsigned char MajorRevision[6];
   unsigned char MinorRevision[6];
   unsigned char ADCScanRate[6];
   float MainADCCalibrationK1[6];
   float MainADCCalibrationK2[6];
   float MainADCCalibrationK3[6];
   float MainADCCalibrationK4[6];
   float MainLoadCellOffset[6];
   float MainLoadCellSensitivity[6];
   float BackupADCCalibrationK1[6];
   float BackupADCCalibrationK2[6];
   float BackupADCCalibrationK3[6];
   float BackupADCCalibrationK4[6];
   float BackupLoadCellOffset[6];
   float BackupLoadCellSensitivity[6];
   int MainCalibrationError[6];
   int BackupCalibrationError[6];
   int UniqueIdCRCError[6];
   int ApplicationTypeMismatch[6];
   int ApplicationMissing[6];
   int ApplicationCRCMismatch[6];
   int OneWireMissing[6];
   int OneWire1Mismatch[6];
   int OneWire2Mismatch[6];
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_HardpointActuatorInfoC()  { std::cout << "m1m3_logevent_HardpointActuatorInfoC()"  << std::endl; }
  ~m1m3_logevent_HardpointActuatorInfoC() { std::cout << "~m1m3_logevent_HardpointActuatorInfoC()"  << std::endl; }
#endif
};

struct m1m3_logevent_ILCCommunicationC
{
  double Timestamp;
  int ActuatorId;
  int CommunicationOk;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_ILCCommunicationC()  { std::cout << "m1m3_logevent_ILCCommunicationC()"  << std::endl; }
  ~m1m3_logevent_ILCCommunicationC() { std::cout << "~m1m3_logevent_ILCCommunicationC()"  << std::endl; }
#endif
};

struct m1m3_logevent_LowerMirrorCompleteC
{
  double Timestamp;
  int Successful;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_LowerMirrorCompleteC()  { std::cout << "m1m3_logevent_LowerMirrorCompleteC()"  << std::endl; }
  ~m1m3_logevent_LowerMirrorCompleteC() { std::cout << "~m1m3_logevent_LowerMirrorCompleteC()"  << std::endl; }
#endif
};

struct m1m3_logevent_RaiseMirrorCompleteC
{
  double Timestamp;
  int Successful;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_RaiseMirrorCompleteC()  { std::cout << "m1m3_logevent_RaiseMirrorCompleteC()"  << std::endl; }
  ~m1m3_logevent_RaiseMirrorCompleteC() { std::cout << "~m1m3_logevent_RaiseMirrorCompleteC()"  << std::endl; }
#endif
};

struct m1m3_logevent_ServoLoopsC
{
  double Timestamp;
  int HardpointChase;
  int HardpointOffloading;
  int OuterLoopTimeOk;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_ServoLoopsC()  { std::cout << "m1m3_logevent_ServoLoopsC()"  << std::endl; }
  ~m1m3_logevent_ServoLoopsC() { std::cout << "~m1m3_logevent_ServoLoopsC()"  << std::endl; }
#endif
};

struct m1m3_logevent_SettingVersionsC
{
  double Timestamp;
  std::string	RecommendedSettingsVersion;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_SettingVersionsC()  { std::cout << "m1m3_logevent_SettingVersionsC()"  << std::endl; }
  ~m1m3_logevent_SettingVersionsC() { std::cout << "~m1m3_logevent_SettingVersionsC()"  << std::endl; }
#endif
};

struct m1m3_logevent_SettingsAppliedC
{
  double Timestamp;
  std::string	Settings;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_SettingsAppliedC()  { std::cout << "m1m3_logevent_SettingsAppliedC()"  << std::endl; }
  ~m1m3_logevent_SettingsAppliedC() { std::cout << "~m1m3_logevent_SettingsAppliedC()"  << std::endl; }
#endif
};

struct m1m3_logevent_StartupChecksC
{
  double Timestamp;
  int ILCVersionsOk;
  int DCAVersionsOk;
  int ILCFirmwareOk;
  int DCAFirmwareOk;
  int ILCStatusOk;
  int DCAStatusOk;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_StartupChecksC()  { std::cout << "m1m3_logevent_StartupChecksC()"  << std::endl; }
  ~m1m3_logevent_StartupChecksC() { std::cout << "~m1m3_logevent_StartupChecksC()"  << std::endl; }
#endif
};

struct m1m3_logevent_StatusChecksC
{
  double Timestamp;
  int ILCResetOk;
  int DCAResetOk;
  int ILCPowerOk;
  int DCAPowerOk;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_StatusChecksC()  { std::cout << "m1m3_logevent_StatusChecksC()"  << std::endl; }
  ~m1m3_logevent_StatusChecksC() { std::cout << "~m1m3_logevent_StatusChecksC()"  << std::endl; }
#endif
};

struct m1m3_logevent_SummaryStateC
{
  double Timestamp;
  int SummaryState;
  int priority;

#ifdef SAL_DEBUG_CSTRUCTS
  m1m3_logevent_SummaryStateC()  { std::cout << "m1m3_logevent_SummaryStateC()"  << std::endl; }
  ~m1m3_logevent_SummaryStateC() { std::cout << "~m1m3_logevent_SummaryStateC()"  << std::endl; }
#endif
};


struct m1m3_commandC
{
      std::string	device;
      std::string	property;
      std::string	action;
      std::string	value;
      std::string	modifiers;

};
struct m1m3_ackcmdC
{
      int 	ack;
      int 	error;
      std::string	result;

};
struct m1m3_logeventC
{
      std::string	message;

};

#endif

