import time
import sys
import numpy
from SALPY_m1m3 import *
import ILCSimulator
import InclinometerSimulator
import DisplaceSimulator
import AccelSimulator
import UDP

forceActuatorTable = [
[0,101,776.782776,0,-89,'SAA',3,1,'NA',-3800,3800,-3800,3800,],
[1,102,1442.56799,0,-89,'DAA',1,17,'+Y',-3800,3800,-3800,3800,],
[2,103,2108.37793,0,-89,'DAA',4,17,'+Y',-3800,3800,-3800,3800,],
[3,104,2774.18799,0,-89,'DAA',2,17,'+Y',-3800,3800,-3800,3800,],
[4,105,3439.99805,0,-89,'DAA',3,17,'+Y',-3800,3800,-3800,3800,],
[5,106,3968.01294,0,-89,'SAA',2,1,'NA',-3800,3800,-3800,3800,],
[6,107,443.86499,-576.60498,-89,'SAA',1,1,'NA',-3800,3800,-3800,3800,],
[7,108,1109.67505,-576.60498,-89,'DAA',4,18,'+Y',-3800,3800,-3800,3800,],
[8,109,1775.48499,-576.60498,-89,'DAA',2,18,'+Y',-3800,3800,-3800,3800,],
[9,110,2441.2959,-576.60498,-89,'DAA',3,18,'+Y',-3800,3800,-3800,3800,],
[10,111,3107.08008,-576.60498,-89,'DAA',1,18,'+Y',-3800,3800,-3800,3800,],
[11,112,3772.89111,-576.60498,-89,'DAA',4,19,'-X',-3800,3800,-3800,3800,],
[12,113,0,-1153.20996,-89,'DAA',2,19,'+Y',-3800,3800,-3800,3800,],
[13,114,776.782776,-1153.20996,-89,'DAA',3,19,'+Y',-3800,3800,-3800,3800,],
[14,115,1442.56799,-1153.20996,-89,'DAA',1,19,'+Y',-3800,3800,-3800,3800,],
[15,116,2108.37793,-1153.20996,-89,'DAA',4,20,'+Y',-3800,3800,-3800,3800,],
[16,117,2774.18799,-1153.20996,-89,'DAA',2,20,'+Y',-3800,3800,-3800,3800,],
[17,118,3439.99805,-1153.20996,-89,'DAA',3,20,'+Y',-3800,3800,-3800,3800,],
[18,119,3900.5,-997.687012,-89,'SAA',2,2,'NA',-3800,3800,-3800,3800,],
[19,120,443.86499,-1729.81995,-89,'DAA',1,20,'+Y',-3800,3800,-3800,3800,],
[20,121,1109.67505,-1729.81995,-89,'DAA',4,21,'+Y',-3800,3800,-3800,3800,],
[21,122,1775.48499,-1729.81995,-89,'DAA',2,21,'+Y',-3800,3800,-3800,3800,],
[22,123,2441.27002,-1729.81995,-89,'DAA',3,21,'+Y',-3800,3800,-3800,3800,],
[23,124,3107.08008,-1729.81995,-89,'DAA',1,21,'+Y',-3800,3800,-3800,3800,],
[24,125,3724.45288,-1517.94995,-89,'SAA',4,1,'NA',-3800,3800,-3800,3800,],
[25,126,0,-2306.41992,-89,'DAA',2,22,'+Y',-3800,3800,-3800,3800,],
[26,127,776.782776,-2306.41992,-89,'DAA',3,22,'+Y',-3800,3800,-3800,3800,],
[27,128,1442.56799,-2306.41992,-89,'DAA',1,22,'-X',-3800,3800,-3800,3800,],
[28,129,2108.37793,-2306.41992,-89,'DAA',4,22,'+Y',-3800,3800,-3800,3800,],
[29,130,2774.18799,-2306.41992,-89,'DAA',2,23,'+Y',-3800,3800,-3800,3800,],
[30,131,3387.9541,-2167.40991,-89,'SAA',3,2,'NA',-3800,3800,-3800,3800,],
[31,132,443.86499,-2883.03003,-89,'DAA',1,23,'+Y',-3800,3800,-3800,3800,],
[32,133,1109.67505,-2883.03003,-89,'DAA',4,23,'+Y',-3800,3800,-3800,3800,],
[33,134,1775.48499,-2883.03003,-89,'DAA',2,24,'+Y',-3800,3800,-3800,3800,],
[34,135,2441.27002,-2883.03003,-89,'DAA',3,23,'-X',-3800,3800,-3800,3800,],
[35,136,2939.36401,-2745.17993,-89,'SAA',4,2,'NA',-3800,3800,-3800,3800,],
[36,137,221.945206,-3459.62988,-89,'DAA',2,25,'+Y',-3800,3800,-3800,3800,],
[37,138,887.72998,-3459.62988,-89,'DAA',3,24,'+Y',-3800,3800,-3800,3800,],
[38,139,1553.54004,-3267.42993,-89,'SAA',1,2,'NA',-3800,3800,-3800,3800,],
[39,140,2089.73389,-3436.38989,-89,'SAA',4,3,'NA',-3800,3800,-3800,3800,],
[40,141,365.734589,-4005.25,-89,'SAA',1,3,'NA',-3800,3800,-3800,3800,],
[41,142,1085.08801,-3872.76001,-89,'SAA',2,3,'NA',-3800,3800,-3800,3800,],
[42,143,1604.01001,-3692.78003,-89,'SAA',3,3,'NA',-3800,3800,-3800,3800,],
[43,207,-443.86499,-576.60498,-89,'SAA',1,4,'NA',-3800,3800,-3800,3800,],
[44,208,-1109.68005,-576.60498,-89,'DAA',4,24,'+Y',-3800,3800,-3800,3800,],
[45,209,-1775.48999,-576.60498,-89,'DAA',2,26,'+Y',-3800,3800,-3800,3800,],
[46,210,-2441.30005,-576.60498,-89,'DAA',3,25,'+Y',-3800,3800,-3800,3800,],
[47,211,-3107.08008,-576.60498,-89,'DAA',1,24,'+Y',-3800,3800,-3800,3800,],
[48,212,-3772.88989,-576.60498,-89,'DAA',4,25,'+X',-3800,3800,-3800,3800,],
[49,214,-776.78302,-1153.20996,-89,'DAA',3,26,'+Y',-3800,3800,-3800,3800,],
[50,215,-1442.56995,-1153.20996,-89,'DAA',1,25,'+Y',-3800,3800,-3800,3800,],
[51,216,-2108.37988,-1153.20996,-89,'DAA',4,26,'NA',-3800,3800,-3800,3800,],
[52,217,-2774.18994,-1153.20996,-89,'DAA',2,27,'+Y',-3800,3800,-3800,3800,],
[53,218,-3440,-1153.20996,-89,'DAA',3,27,'+Y',-3800,3800,-3800,3800,],
[54,219,-3900.5,-997.687012,-89,'SAA',2,4,'+Y',-3800,3800,-3800,3800,],
[55,220,-443.86499,-1729.81995,-89,'DAA',1,26,'+Y',-3800,3800,-3800,3800,],
[56,221,-1109.68005,-1729.81995,-89,'DAA',4,27,'+Y',-3800,3800,-3800,3800,],
[57,222,-1775.48999,-1729.81995,-89,'DAA',2,28,'+Y',-3800,3800,-3800,3800,],
[58,223,-2441.27002,-1729.81995,-89,'DAA',3,28,'+Y',-3800,3800,-3800,3800,],
[59,224,-3107.08008,-1729.81995,-89,'DAA',1,27,'+Y',-3800,3800,-3800,3800,],
[60,225,-3724.44995,-1517.94995,-89,'SAA',4,4,'NA',-3800,3800,-3800,3800,],
[61,227,-776.78302,-2306.41992,-89,'DAA',3,29,'+Y',-3800,3800,-3800,3800,],
[62,228,-1442.56995,-2306.41992,-89,'DAA',1,28,'+X',-3800,3800,-3800,3800,],
[63,229,-2108.37988,-2306.41992,-89,'DAA',4,28,'+Y',-3800,3800,-3800,3800,],
[64,230,-2774.18994,-2306.41992,-89,'DAA',2,29,'+Y',-3800,3800,-3800,3800,],
[65,231,-3387.94995,-2167.40991,-89,'SAA',3,4,'NA',-3800,3800,-3800,3800,],
[66,232,-443.86499,-2883.03003,-89,'DAA',1,29,'+Y',-3800,3800,-3800,3800,],
[67,233,-1109.68005,-2883.03003,-89,'DAA',4,29,'+Y',-3800,3800,-3800,3800,],
[68,234,-1775.48999,-2883.03003,-89,'DAA',2,30,'+Y',-3800,3800,-3800,3800,],
[69,235,-2441.27002,-2883.03003,-89,'DAA',3,30,'+X',-3800,3800,-3800,3800,],
[70,236,-2939.36011,-2745.17993,-89,'SAA',4,5,'NA',-3800,3800,-3800,3800,],
[71,237,-221.945007,-3459.62988,-89,'DAA',2,31,'+Y',-3800,3800,-3800,3800,],
[72,238,-887.72998,-3459.62988,-89,'DAA',3,31,'+Y',-3800,3800,-3800,3800,],
[73,239,-1553.54004,-3267.42993,-89,'SAA',1,5,'NA',-3800,3800,-3800,3800,],
[74,240,-2089.72998,-3436.38989,-89,'SAA',4,6,'NA',-3800,3800,-3800,3800,],
[75,241,-365.734985,-4005.25,-89,'SAA',1,6,'NA',-3800,3800,-3800,3800,],
[76,242,-1085.08997,-3872.76001,-89,'SAA',2,5,'NA',-3800,3800,-3800,3800,],
[77,243,-1604.01001,-3692.78003,-89,'SAA',3,5,'NA',-3800,3800,-3800,3800,],
[78,301,-776.78302,0,-89,'SAA',3,6,'NA',-3800,3800,-3800,3800,],
[79,302,-1442.56995,0,-89,'DAA',1,30,'+Y',-3800,3800,-3800,3800,],
[80,303,-2108.37988,0,-89,'DAA',4,30,'+Y',-3800,3800,-3800,3800,],
[81,304,-2774.18994,0,-89,'DAA',2,32,'+Y',-3800,3800,-3800,3800,],
[82,305,-3440,0,-89,'DAA',3,32,'+Y',-3800,3800,-3800,3800,],
[83,306,-3968.01001,0,-89,'SAA',2,6,'NA',-3800,3800,-3800,3800,],
[84,307,-443.86499,576.605408,-89,'SAA',1,7,'NA',-3800,3800,-3800,3800,],
[85,308,-1109.68005,576.605408,-89,'DAA',4,31,'+Y',-3800,3800,-3800,3800,],
[86,309,-1775.48999,576.605408,-89,'DAA',2,33,'+Y',-3800,3800,-3800,3800,],
[87,310,-2441.30005,576.605408,-89,'DAA',3,33,'+Y',-3800,3800,-3800,3800,],
[88,311,-3107.08008,576.605408,-89,'DAA',1,31,'-Y',-3800,3800,-3800,3800,],
[89,312,-3772.88989,576.605408,-89,'DAA',4,32,'+X',-3800,3800,-3800,3800,],
[90,313,0,1153.21106,-89,'DAA',2,34,'+Y',-3800,3800,-3800,3800,],
[91,314,-776.78302,1153.21106,-89,'DAA',3,34,'+Y',-3800,3800,-3800,3800,],
[92,315,-1442.56995,1153.21106,-89,'DAA',1,32,'+Y',-3800,3800,-3800,3800,],
[93,316,-2108.37988,1153.21106,-89,'DAA',4,33,'+Y',-3800,3800,-3800,3800,],
[94,317,-2774.18994,1153.21106,-89,'DAA',2,35,'+Y',-3800,3800,-3800,3800,],
[95,318,-3440,1153.21106,-89,'DAA',3,35,'+Y',-3800,3800,-3800,3800,],
[96,319,-3900.5,997.686584,-89,'SAA',2,7,'NA',-3800,3800,-3800,3800,],
[97,320,-443.86499,1729.81604,-89,'DAA',1,33,'+Y',-3800,3800,-3800,3800,],
[98,321,-1109.68005,1729.81604,-89,'DAA',4,34,'+Y',-3800,3800,-3800,3800,],
[99,322,-1775.48999,1729.81604,-89,'DAA',2,36,'+Y',-3800,3800,-3800,3800,],
[100,323,-2441.27002,1729.81604,-89,'DAA',3,36,'+Y',-3800,3800,-3800,3800,],
[101,324,-3107.08008,1729.81604,-89,'DAA',1,34,'+Y',-3800,3800,-3800,3800,],
[102,325,-3724.44995,1517.95496,-89,'SAA',4,7,'NA',-3800,3800,-3800,3800,],
[103,326,0,2306.42212,-89,'DAA',2,37,'+Y',-3800,3800,-3800,3800,],
[104,327,-776.78302,2306.42212,-89,'DAA',3,37,'+Y',-3800,3800,-3800,3800,],
[105,328,-1442.56995,2306.42212,-89,'DAA',1,35,'+X',-3800,3800,-3800,3800,],
[106,329,-2108.37988,2306.42212,-89,'DAA',4,35,'+Y',-3800,3800,-3800,3800,],
[107,330,-2774.18994,2306.42212,-89,'DAA',2,38,'+Y',-3800,3800,-3800,3800,],
[108,331,-3387.94995,2167.40698,-89,'SAA',3,7,'NA',-3800,3800,-3800,3800,],
[109,332,-443.86499,2883.0271,-89,'DAA',1,36,'+Y',-3800,3800,-3800,3800,],
[110,333,-1109.68005,2883.0271,-89,'DAA',4,36,'+Y',-3800,3800,-3800,3800,],
[111,334,-1775.48999,2883.0271,-89,'DAA',2,39,'-Y',-3800,3800,-3800,3800,],
[112,335,-2441.27002,2883.0271,-89,'DAA',3,38,'+X',-3800,3800,-3800,3800,],
[113,336,-2939.36011,2745.18091,-89,'SAA',4,8,'NA',-3800,3800,-3800,3800,],
[114,337,-221.945007,3459.63208,-89,'DAA',2,40,'+Y',-3800,3800,-3800,3800,],
[115,338,-887.72998,3459.63208,-89,'DAA',3,39,'+Y',-3800,3800,-3800,3800,],
[116,339,-1553.54004,3267.43091,-89,'SAA',1,8,'NA',-3800,3800,-3800,3800,],
[117,340,-2089.72998,3436.39111,-89,'SAA',4,9,'NA',-3800,3800,-3800,3800,],
[118,341,-365.734985,4005.25,-89,'SAA',1,9,'NA',-3800,3800,-3800,3800,],
[119,342,-1085.08997,3872.76294,-89,'SAA',2,8,'NA',-3800,3800,-3800,3800,],
[120,343,-1604.01001,3692.77905,-89,'SAA',3,8,'NA',-3800,3800,-3800,3800,],
[121,407,443.86499,576.605408,-89,'SAA',1,10,'NA',-3800,3800,-3800,3800,],
[122,408,1109.67505,576.605408,-89,'DAA',4,37,'+Y',-3800,3800,-3800,3800,],
[123,409,1775.48499,576.605408,-89,'DAA',2,41,'+Y',-3800,3800,-3800,3800,],
[124,410,2441.2959,576.605408,-89,'DAA',3,40,'+Y',-3800,3800,-3800,3800,],
[125,411,3107.08008,576.605408,-89,'DAA',1,37,'-Y',-3800,3800,-3800,3800,],
[126,412,3772.89111,576.605408,-89,'DAA',4,38,'-X',-3800,3800,-3800,3800,],
[127,414,776.782776,1153.21106,-89,'DAA',3,41,'+Y',-3800,3800,-3800,3800,],
[128,415,1442.56799,1153.21106,-89,'DAA',1,38,'+Y',-3800,3800,-3800,3800,],
[129,416,2108.37793,1153.21106,-89,'DAA',4,39,'+Y',-3800,3800,-3800,3800,],
[130,417,2774.18799,1153.21106,-89,'DAA',2,42,'+Y',-3800,3800,-3800,3800,],
[131,418,3439.99805,1153.21106,-89,'DAA',3,42,'+Y',-3800,3800,-3800,3800,],
[132,419,3900.5,997.686584,-89,'SAA',2,9,'NA',-3800,3800,-3800,3800,],
[133,420,443.86499,1729.81604,-89,'DAA',1,39,'+Y',-3800,3800,-3800,3800,],
[134,421,1109.67505,1729.81604,-89,'DAA',4,40,'+Y',-3800,3800,-3800,3800,],
[135,422,1775.48499,1729.81604,-89,'DAA',2,43,'+Y',-3800,3800,-3800,3800,],
[136,423,2441.27002,1729.81604,-89,'DAA',3,43,'+Y',-3800,3800,-3800,3800,],
[137,424,3107.08008,1729.81604,-89,'DAA',1,40,'+Y',-3800,3800,-3800,3800,],
[138,425,3724.45288,1517.95496,-89,'SAA',4,10,'NA',-3800,3800,-3800,3800,],
[139,427,776.782776,2306.42212,-89,'DAA',3,44,'+Y',-3800,3800,-3800,3800,],
[140,428,1442.56799,2306.42212,-89,'DAA',1,41,'-X',-3800,3800,-3800,3800,],
[141,429,2108.37793,2306.42212,-89,'DAA',4,41,'+Y',-3800,3800,-3800,3800,],
[142,430,2774.18799,2306.42212,-89,'DAA',2,44,'+Y',-3800,3800,-3800,3800,],
[143,431,3387.9541,2167.40698,-89,'SAA',3,9,'NA',-3800,3800,-3800,3800,],
[144,432,443.86499,2883.0271,-89,'DAA',1,42,'+Y',-3800,3800,-3800,3800,],
[145,433,1109.67505,2883.0271,-89,'DAA',4,42,'+Y',-3800,3800,-3800,3800,],
[146,434,1775.48499,2883.0271,-89,'DAA',2,45,'-Y',-3800,3800,-3800,3800,],
[147,435,2441.27002,2883.0271,-89,'DAA',3,45,'-X',-3800,3800,-3800,3800,],
[148,436,2939.36401,2745.18091,-89,'SAA',4,11,'NA',-3800,3800,-3800,3800,],
[149,437,221.945206,3459.63208,-89,'DAA',2,46,'+Y',-3800,3800,-3800,3800,],
[150,438,887.72998,3459.63208,-89,'DAA',3,46,'+Y',-3800,3800,-3800,3800,],
[151,439,1553.54004,3267.43091,-89,'SAA',1,11,'NA',-3800,3800,-3800,3800,],
[152,440,2089.73389,3436.39111,-89,'SAA',4,12,'NA',-3800,3800,-3800,3800,],
[153,441,365.734589,4005.25,-89,'SAA',1,12,'NA',-3800,3800,-3800,3800,],
[154,442,1085.08801,3872.76294,-89,'SAA',2,10,'NA',-3800,3800,-3800,3800,],
[155,443,1604.01001,3692.77905,-89,'SAA',3,10,'NA',-3800,3800,-3800,3800,]
]
forceActuatorTableIndexIndex = 0
forceActuatorTableIDIndex = 1
forceActuatorTableXPositionIndex = 2
forceActuatorTableYPositionIndex = 3
forceActuatorTableZPositionIndex = 4
forceActuatorTableTypeIndex = 5
forceActuatorTableSubnetIndex = 6
forceActuatorTableAddressIndex = 7
forceActuatorTableOrientationIndex = 8

hardpointActuatorTable = [
[0,1,776.782776,0,-89,5,1,0,3],
[1,2,1442.56799,0,-89,5,2,0,3],
[2,3,2108.37793,0,-89,5,3,0,3],
[3,4,2774.18799,0,-89,5,4,0,3],
[4,5,3439.99805,0,-89,5,5,0,3],
[5,6,3968.01294,0,-89,5,6,0,3],
]

hardpointActuatorTableIndexIndex = 0
hardpointActuatorTableIDIndex = 1
hardpointActuatorTableXPositionIndex = 2
hardpointActuatorTableYPositionIndex = 3
hardpointActuatorTableZPositionIndex = 4
hardpointActuatorTableSubnetIndex = 5
hardpointActuatorTableAddressIndex = 6

ipAddress = '140.252.24.124'

OfflineState             = 0x00000000
StandbyState             = 0x01000000
DisabledState            = 0x02000000
EnabledState             = 0x03000000
ParkedState              = 0x03000001
RaisingState             = 0x03000002
ActiveState              = 0x03000003
LoweringState            = 0x03000004
EngineeringState         = 0x03010000
ParkedEngineeringState   = 0x03010001
RaisingEngineeringState  = 0x03010002
ActiveEngineeringState   = 0x03010003
LoweringEngineeringState = 0x03010004
FaultState               = 0xFFFFFFFF

print("Starting Simulator")
ilcSim = ILCSimulator.ILCSimulator()
inclinSim = InclinometerSimulator.InclinometerSimulator()
displaceSim = DisplaceSimulator.DisplacementSimulator()
accelSim = AccelSimulator.AccelSimulator()

udpClientSubnetA = UDP.UDP(ipAddress, 5006)
udpClientSubnetB = UDP.UDP(ipAddress, 5007)
udpClientSubnetC = UDP.UDP(ipAddress, 5008)
udpClientSubnetD = UDP.UDP(ipAddress, 5009)
udpClientSubnetE = UDP.UDP(ipAddress, 5005)
udpClientInclin = UDP.UDP(ipAddress, 5010)
udpClientDisplace = UDP.UDP(ipAddress, 5011)
udpClientAccel = UDP.UDP(ipAddress, 5012)

print("Starting SAL")
salM1M3 = SAL_m1m3()
salM1M3.setDebugLevel(0)

print("Initializing topics")
startTime = time.time()
salM1M3.salEvent("m1m3_logevent_SummaryState")
salM1M3.salCommand("m1m3_command_Start")
salM1M3.salCommand("m1m3_command_Enable")
salM1M3.salCommand("m1m3_command_EnterEngineering")
salM1M3.salCommand("m1m3_command_RaiseM1M3")
salM1M3.salCommand("m1m3_command_AbortRaiseM1M3")
salM1M3.salCommand("m1m3_command_LowerM1M3")
salM1M3.salCommand("m1m3_command_ExitEngineering")
salM1M3.salCommand("m1m3_command_Disable")
salM1M3.salCommand("m1m3_command_Standby")
salM1M3.salCommand("m1m3_command_Shutdown")
endTime = time.time()

print("Initialization complete, took %0.3f seconds" % (endTime - startTime))

def subnetToUDPClient(subnet):
    if subnet == 1:
        return udpClientSubnetA
    elif subnet == 2:
        return udpClientSubnetB
    elif subnet == 3:
        return udpClientSubnetC
    elif subnet == 4:
        return udpClientSubnetD
    elif subnet == 5:
        return udpClientSubnetE
    raise ValueError()

def checkEquals(actual, expected, description):
    result = actual == expected
    print("%s : %s" % (description, result))
    return result
    
def afterCommand():
    time.sleep(1)
    
def issueStartCommand():
    #print("-- Start Command")
    data = m1m3_command_StartC()
    data.Start = True
    data.SettingsToApply = "Default"
    cmdId = salM1M3.issueCommand_Start(data)
    salM1M3.waitForCompletion_Start(cmdId, 10)
    afterCommand()
    
def issueEnableCommand():
    #print("-- Enable Command")
    data = m1m3_command_EnableC()
    data.Enable = True
    cmdId = salM1M3.issueCommand_Enable(data)
    salM1M3.waitForCompletion_Enable(cmdId, 10)
    afterCommand()
    
def issueEnterEngineeringCommand():
    #print("-- Enter Engineering Command")
    data = m1m3_command_EnterEngineeringC()
    data.EnterEngineering = True
    cmdId = salM1M3.issueCommand_EnterEngineering(data)
    salM1M3.waitForCompletion_EnterEngineering(cmdId, 10)
    afterCommand()

def issueRaiseM1M3Command():
    #print("-- Raise M1M3 Command")
    data = m1m3_command_RaiseM1M3C()
    data.RaiseM1M3 = True
    cmdId = salM1M3.issueCommand_RaiseM1M3(data)
    salM1M3.waitForCompletion_RaiseM1M3(cmdId, 10)
    afterCommand()
    
def issueAbortRaiseM1M3Command():
    #print("-- Abort Raise M1M3 Command")
    data = m1m3_command_AbortRaiseM1M3C()
    data.AbortRaiseM1M3 = True
    cmdId = salM1M3.issueCommand_AbortRaiseM1M3(data)
    salM1M3.waitForCompletion_AbortRaiseM1M3(cmdId, 10)
    afterCommand()
    
def issueLowerM1M3Command():
    #print("-- Lower M1M3 Command")
    data = m1m3_command_LowerM1M3C()
    data.LowerM1M3 = True
    cmdId = salM1M3.issueCommand_LowerM1M3(data)
    salM1M3.waitForCompletion_LowerM1M3(cmdId, 10)
    afterCommand()
    
def issueExitEngineeringCommand():
    #print("-- Exit Engineering Command")
    data = m1m3_command_ExitEngineeringC()
    data.ExitEngineering = True
    cmdId = salM1M3.issueCommand_ExitEngineering(data)
    salM1M3.waitForCompletion_ExitEngineering(cmdId, 10)
    afterCommand()
    
def issueDisableCommand():
    #print("-- Disable Command")
    data = m1m3_command_DisableC()
    data.Disable = True
    cmdId = salM1M3.issueCommand_Disable(data)
    salM1M3.waitForCompletion_Disable(cmdId, 10)
    afterCommand()
    
def issueStandbyCommand():
    #print("-- Standby Command")
    data = m1m3_command_StandbyC()
    data.Standby = True
    cmdId = salM1M3.issueCommand_Standby(data)
    salM1M3.waitForCompletion_Standby(cmdId, 10)
    afterCommand()
    
def issueShutdownCommand():
    #print("-- Shutdown Command")
    data = m1m3_command_ShutdownC()
    data.Shutdown = True
    cmdId = salM1M3.issueCommand_Shutdown(data)
    salM1M3.waitForCompletion_Shutdown(cmdId, 10)
    afterCommand()

def checkSummaryStateEquals(expected, description):
    data = m1m3_logevent_SummaryStateC()
    salM1M3.getEvent_SummaryState(data)
    return checkEquals(data.SummaryState, expected, description)
    
def checkNoSummaryState(description):
    data = m1m3_logevent_SummaryStateC()
    retValue = salM1M3.getEvent_SummaryState(data)
    result = retValue != 0
    print("%s : %s" % (description, result))
    return result
    
def waitForSummaryStateEquals(expected, timeout, description):
    startTime = time.time()
    data = m1m3_logevent_SummaryStateC()
    while True:
        retVal = salM1M3.getEvent_SummaryState(data)
        if retVal == 0:
            if data.SummaryState == expected:
                return checkEquals(data.SummaryState, expected, description)
        elif (time.time() - startTime) >= timeout:
            return checkEquals(data.SummaryState, expected, description)

##################################    
print("----------------------------------------------------------------------")
print("-- StandbyState Verification")
print("----------------------------------------------------------------------")
checkSummaryStateEquals(StandbyState, "Start from StandbyState")
# Verify Start command transitions to DisabledState
issueStartCommand()
checkSummaryStateEquals(DisabledState, "Transition from StandbyState to DisabledState after Start command")
issueStandbyCommand()
checkSummaryStateEquals(StandbyState, "Transition from DisabledState to StandbyState after Standby command")
# Verify Enable command does nothing
issueEnableCommand()
checkNoSummaryState("No transition from StandbyState after Enable command")
# Verify RaiseM1M3 command does nothing
issueRaiseM1M3Command()
checkNoSummaryState("No transition from StandbyState after RaiseM1M3 command")
# Verify AbortRaiseM1M3 command does nothing
issueAbortRaiseM1M3Command()
checkNoSummaryState("No transition from StandbyState after AbortRaiseM1M3 command")
# Verify LowerM1M3 command does nothing
issueLowerM1M3Command()
checkNoSummaryState("No transition from StandbyState after LowerM1M3 command")
# Verify EnterEngineering command does nothing
issueEnterEngineeringCommand()
checkNoSummaryState("No transition from StandbyState after EnterEngineering command")
# Verify ExitEngineering command does nothing
issueExitEngineeringCommand()
checkNoSummaryState("No transition from StandbyState after ExitEngineering command")
# Verify Disable command does nothing
issueDisableCommand()
checkNoSummaryState("No transition from StandbyState after Disable command")
# Verify Standby command does nothing
issueStandbyCommand()
checkNoSummaryState("No transition from StandbyState after Standby command")
# Verify Shutdown command transitions to OfflineState
#issueShutdownCommand()
#checkSummaryStateEquals(OfflineState, "Transition from StandbyState to OfflineState after Shutdown command")
##################################  
    
##################################
print("----------------------------------------------------------------------")
print("-- DisabledState Verification")
print("----------------------------------------------------------------------")
# Get into the DisabledState 
issueStartCommand()
checkSummaryStateEquals(DisabledState, "Transition from StandbyState to DisabledState after Start command")
# Verify Start command does nothing
issueStartCommand()
checkNoSummaryState("No transition from DisabledState after Start command")
# Verify Enable transitions to the EnabledState
issueEnableCommand()
checkSummaryStateEquals(ParkedState, "Transition from DisabledState to ParkedState after Enable command")
issueDisableCommand()
checkSummaryStateEquals(DisabledState, "Transition from ParkedState to DisabledState after Disable command")
# Verify RaiseM1M3 command does nothing
issueRaiseM1M3Command()
checkNoSummaryState("No transition from DisabledState after RaiseM1M3 command")
# Verify AbortRaiseM1M3 command does nothing
issueAbortRaiseM1M3Command()
checkNoSummaryState("No transition from DisabledState after AbortRaiseM1M3 command")
# Verify LowerM1M3 command does nothing
issueLowerM1M3Command()
checkNoSummaryState("No transition from DisabledState after LowerM1M3 command")
# Verify EnterEngineering command does nothing
issueEnterEngineeringCommand()
checkNoSummaryState("No transition from DisabledState after EnterEngineering command")
# Verify ExitEngineering command does nothing
issueExitEngineeringCommand()
checkNoSummaryState("No transition from DisabledState after ExitEngineering command")
# Verify Disable command does nothing
issueDisableCommand()
checkNoSummaryState("No transition from DisabledState after Disable command")
# Verify Standby command does nothing
issueStandbyCommand()
checkSummaryStateEquals(StandbyState, "Transition from DisabledState to StandbyState after Standby command")
issueStartCommand()
checkSummaryStateEquals(DisabledState, "Transition from StandbyState to DisabledState after Start command")
# Verify Shutdown command does nothing
issueShutdownCommand()
checkNoSummaryState("No transition from DisabledState after Shutdown command")
##################################

##################################
print("----------------------------------------------------------------------")
print("-- ParkedState Verification")
print("----------------------------------------------------------------------")
# Get into ParkedState
issueEnableCommand()
checkSummaryStateEquals(ParkedState, "Transition from DisabledState to ParkedState after Enable command")
# Verify Start command does nothing
issueStartCommand()
checkNoSummaryState("No transition from ParkedState after Start command")
# Verify Enable command does nothing
issueEnableCommand()
checkNoSummaryState("No transition from ParkedState after Enable command")
# Verify RaiseM1M3 command transitions to RaisingState
issueRaiseM1M3Command()
checkSummaryStateEquals(RaisingState, "Transition from ParkedState to RaisingState after RaiseM1M3 command")
issueAbortRaiseM1M3Command()
checkSummaryStateEquals(LoweringState, "Transition from RaisingState to LoweringState after RaiseM1M3 command")
waitForSummaryStateEquals(ParkedState, 300, "Transition from LoweringState to ParkedState after LoweringState completes")
# Verify AbortRaiseM1M3 command does nothing
issueAbortRaiseM1M3Command()
checkNoSummaryState("No transition from ParkedState after AbortRaiseM1M3 command")
# Verify LowerM1M3 command does nothing
issueLowerM1M3Command()
checkNoSummaryState("No transition from ParkedState after LowerM1M3 command")
# Verify EnterEngineering command does nothing
issueEnterEngineeringCommand()
checkSummaryStateEquals(ParkedEngineeringState, "Transition from ParkedState to ParkedEngineeringState after EnterEngineering command")
issueExitEngineeringCommand()
checkSummaryStateEquals(ParkedState, "Transition from ParkedEngineeringState to ParkedState after ExitEngineering command")
# Verify ExitEngineering command does nothing
issueExitEngineeringCommand()
checkNoSummaryState("No transition from ParkedState after ExitEngineering command")
# Verify Disable command transitions to DisabledState
issueDisableCommand()
checkSummaryStateEquals(DisabledState, "Transition from ParkedState to DisabledState after Disable command")
issueEnableCommand()
checkSummaryStateEquals(ParkedState, "Transition from DisabledState to ParkedState after Enable command")
# Verify Standby command does nothing
issueStandbyCommand()
checkNoSummaryState("No transition from ParkedState after Standby command")
# Verify Shutdown command does nothing
issueShutdownCommand()
checkNoSummaryState("No transition from ParkedState after Shutdown command")
##################################  

##################################  
print("----------------------------------------------------------------------")
print("-- RaisingState Verification")
print("----------------------------------------------------------------------")
# Transition to RaisingState
issueRaiseM1M3Command()
checkSummaryStateEquals(RaisingState, "Transition from ParkedState to RaisingState after RaiseM1M3")
# Verify Start command does nothing
issueStartCommand()
checkNoSummaryState("No transition from RaisingState after Start command")
# Verify Enable command does nothing
issueEnableCommand()
checkNoSummaryState("No transition from RaisingState after Enable command")
# Verify RaiseM1M3 command does nothing
issueRaiseM1M3Command()
checkNoSummaryState("No transition from RaisingState after RaiseM1M3 command")
# Verify LowerM1M3 command does nothing
issueLowerM1M3Command()
checkNoSummaryState("No transition from RaisingState after LowerM1M3 command")
# Verify EnterEngineering command does nothing
issueEnterEngineeringCommand()
checkNoSummaryState("No transition from RaisingState after EnterEngineering command")
# Verify ExitEngineering command does nothing
issueExitEngineeringCommand()
checkNoSummaryState("No transition from RaisingState after ExitEngineering command")
# Verify Disable command does nothing
issueDisableCommand()
checkNoSummaryState("No transition from RaisingState after Disable command")
# Verify Standby command does nothing
issueStandbyCommand()
checkNoSummaryState("No transition from RaisingState after Standby command")
# Verify Shutdown command does nothing
issueShutdownCommand()
checkNoSummaryState("No transition from RaisingState after Shutdown command")
# Verify AbortRaiseM1M3 command works
issueAbortRaiseM1M3Command()
checkSummaryStateEquals(LoweringState, "Transition from RaisingState to LoweringState after AbortRaiseM1M3 command")
waitForSummaryStateEquals(ParkedState, 300, "Transition from LoweringState to ParkedState after LoweringState completes")
##################################  

##################################
print("----------------------------------------------------------------------")
print("-- ActiveState Verification")
print("----------------------------------------------------------------------")
# Get into ActiveState
issueRaiseM1M3Command()
checkSummaryStateEquals(RaisingState, "Transition from ParkedState to RaisingState after RaiseM1M3")
waitForSummaryStateEquals(ActiveState, 300, "Transition from RaisingState to ActiveState after RaisingState completes")
# Verify Start command does nothing
issueStartCommand()
checkNoSummaryState("No transition from ActiveState after Start command")
# Verify Enable command does nothing
issueEnableCommand()
checkNoSummaryState("No transition from ActiveState after Enable command")
# Verify RaiseM1M3 command does nothing
issueRaiseM1M3Command()
checkNoSummaryState("No transition from ActiveState after RaiseM1M3 command")
# Verify AbortRaiseM1M3 command does nothing
issueAbortRaiseM1M3Command()
checkNoSummaryState("No transition from ActiveState after AbortRaiseM1M3 command")
# Verify EnterEngineering command transitions to ActiveEngineeringState
issueEnterEngineeringCommand()
checkSummaryStateEquals(ActiveEngineeringState, "Transition from ActiveState to ActiveEngineeringState after EnterEngineering command")
issueExitEngineeringCommand()
checkSummaryStateEquals(ActiveState, "Transition from ActiveEngineeringState to ActiveState after ExitEngineering command")
# Verify ExitEngineering command does nothing
issueExitEngineeringCommand()
checkNoSummaryState("No transition from ActiveState after ExitEngineering command")
# Verify Disable command does nothing
issueDisableCommand()
checkNoSummaryState("No transition from ActiveState after Disable command")
# Verify Standby command does nothing
issueStandbyCommand()
checkNoSummaryState("No transition from ActiveState after Standby command")
# Verify Shutdown command does nothing
issueShutdownCommand()
checkNoSummaryState("No transition from ActiveState after Shutdown command")
# Verify LowerM1M3 command transitions to LoweringState
issueLowerM1M3Command()
checkSummaryStateEquals(LoweringState, "Transition from ActiveState to LoweringState after LowerM1M3")
waitForSummaryStateEquals(ParkedState, 300, "Transition from LoweringState to ParkedState after LoweringState completes")
##################################  

##################################  
print("----------------------------------------------------------------------")
print("-- Lowering Verification")
print("----------------------------------------------------------------------")
# Get to LoweringState
issueRaiseM1M3Command()
checkSummaryStateEquals(RaisingState, "Transition from ParkedState to RaisingState after RaiseM1M3")
waitForSummaryStateEquals(ActiveState, 300, "Transition from RaisingState to ActiveState after RaisingState completes")
issueLowerM1M3Command()
checkSummaryStateEquals(LoweringState, "Transition from ActiveState to LoweringState after LowerM1M3")
# Verify Start command does nothing
issueStartCommand()
checkNoSummaryState("No transition from LoweringState after Start command")
# Verify Enable command does nothing
issueEnableCommand()
checkNoSummaryState("No transition from LoweringState after Enable command")
# Verify RaiseM1M3 command does nothing
issueRaiseM1M3Command()
checkNoSummaryState("No transition from LoweringState after RaiseM1M3 command")
# Verify AbortRaiseM1M3 command does nothing
issueAbortRaiseM1M3Command()
checkNoSummaryState("No transition from LoweringState after AbortRaiseM1M3 command")
# Verify LowerM1M3 command does nothing
issueLowerM1M3Command()
checkNoSummaryState("No transition from LoweringState after LowerM1M3 command")
# Verify EnterEngineering command does nothing
issueEnterEngineeringCommand()
checkNoSummaryState("No transition from LoweringState after EnterEngineering command")
# Verify ExitEngineering command does nothing
issueExitEngineeringCommand()
checkNoSummaryState("No transition from LoweringState after ExitEngineering command")
# Verify Disable command does nothing
issueDisableCommand()
checkNoSummaryState("No transition from LoweringState after Disable command")
# Verify Standby command does nothing
issueStandbyCommand()
checkNoSummaryState("No transition from LoweringState after Standby command")
# Verify Shutdown command does nothing
issueShutdownCommand()
checkNoSummaryState("No transition from LoweringState after Shutdown command")
# Verify AbortRaiseM1M3 command works
waitForSummaryStateEquals(ParkedState, 300, "Transition from LoweringState to ParkedState after LoweringState completes")
##################################  

##################################
print("----------------------------------------------------------------------")
print("-- ParkedEngineeringState Verification")
print("----------------------------------------------------------------------")
# Get into the ParkedEngineeringState 
issueEnterEngineeringCommand()
checkSummaryStateEquals(ParkedEngineeringState, "Transition from ParkedState to ParkedEngineeringState after EnterEngineering command")
# Verify Start command does nothing
issueStartCommand()
checkNoSummaryState("No transition from ParkedEngineeringState after Start command")
# Verify Enable command does nothing
issueEnableCommand()
checkNoSummaryState("No transition from ParkedEngineeringState after Start command")
# Verify RaiseM1M3 command transitions to RaiseM1M3
issueRaiseM1M3Command()
checkSummaryStateEquals(RaisingEngineeringState, "Transition from ParkedEngineeringState to RaisingEngineeringState after RaiseM1M3 command")
issueAbortRaiseM1M3Command()
checkSummaryStateEquals(LoweringEngineeringState, "Transition from RaisingEngineeringState to LoweringEngineeringState after AbortRaiseM1M3 command")
waitForSummaryStateEquals(ParkedEngineeringState, 300, "Transition from LoweringEngineeringState to ParkedEngineeringState after LoweringEngineeringState completes")
# Verify AbortRaiseM1M3 command does nothing
issueAbortRaiseM1M3Command()
checkNoSummaryState("No transition from ParkedEngineeringState after AbortRaiseM1M3 command")
# Verify LowerM1M3 command does nothing
issueLowerM1M3Command()
checkNoSummaryState("No transition from ParkedEngineeringState after LowerM1M3 command")
# Verify EnterEngineering command does nothing
issueEnterEngineeringCommand()
checkNoSummaryState("No transition from ParkedEngineeringState after EnterEngineering command")
# Verify ExitEngineering command transitions to ParkedState
issueExitEngineeringCommand()
checkSummaryStateEquals(ParkedState, "Transition from ParkedEngineeringState to ParkedState after ExitEngineering command")
issueEnterEngineeringCommand()
checkSummaryStateEquals(ParkedEngineeringState, "Transition from ParkedState to ParkedEngineeringState after EnterEngineering command")
# Verify Disable command transitions to DisabledState
issueDisableCommand()
checkSummaryStateEquals(DisabledState, "Transition from ParkedEngineeringState to DisabledState after Disable command")
issueEnableCommand()
checkSummaryStateEquals(ParkedState, "Transition from DisabledState to ParkedState after Enable command")
issueEnterEngineeringCommand()
checkSummaryStateEquals(ParkedEngineeringState, "Transition from ParkedState to ParkedEngineeringState after EnterEngineering command")
# Verify Standby command does nothing
issueStandbyCommand()
checkNoSummaryState("No transition from ParkedEngineeringState after Standby command")
# Verify Shutdown command does nothing
issueShutdownCommand()
checkNoSummaryState("No transition from ParkedEngineeringState after Shutdown command")
##################################

##################################  
print("----------------------------------------------------------------------")
print("-- RaisingEngineeringState Verification")
print("----------------------------------------------------------------------")
# Transition to RaisingEngineeringState
issueRaiseM1M3Command()
checkSummaryStateEquals(RaisingEngineeringState, "Transition from ParkedEngineeringState to RaisingEngineeringState after RaiseM1M3")
# Verify Start command does nothing
issueStartCommand()
checkNoSummaryState("No transition from RaisingEngineeringState after Start command")
# Verify Enable command does nothing
issueEnableCommand()
checkNoSummaryState("No transition from RaisinRaisingEngineeringStategState after Enable command")
# Verify RaiseM1M3 command does nothing
issueRaiseM1M3Command()
checkNoSummaryState("No transition from RaisingEngineeringState after RaiseM1M3 command")
# Verify LowerM1M3 command does nothing
issueLowerM1M3Command()
checkNoSummaryState("No transition from RaisingEngineeringState after LowerM1M3 command")
# Verify EnterEngineering command does nothing
issueEnterEngineeringCommand()
checkNoSummaryState("No transition from RaisingEngineeringState after EnterEngineering command")
# Verify ExitEngineering command does nothing
issueExitEngineeringCommand()
checkNoSummaryState("No transition from RaisingEngineeringState after ExitEngineering command")
# Verify Disable command does nothing
issueDisableCommand()
checkNoSummaryState("No transition from RaisingEngineeringState after Disable command")
# Verify Standby command does nothing
issueStandbyCommand()
checkNoSummaryState("No transition from RaisingEngineeringState after Standby command")
# Verify Shutdown command does nothing
issueShutdownCommand()
checkNoSummaryState("No transition from RaisingEngineeringState after Shutdown command")
# Verify AbortRaiseM1M3 command works
issueAbortRaiseM1M3Command()
checkSummaryStateEquals(LoweringEngineeringState, "Transition from RaisingEngineeringState to LoweringEngineeringState after AbortRaiseM1M3 command")
waitForSummaryStateEquals(ParkedEngineeringState, 300, "Transition from LoweringEngineeringState to ParkedEngineeringState after LoweringEngineeringState completes")
##################################  

##################################
print("----------------------------------------------------------------------")
print("-- ActiveEngineeringState Verification")
print("----------------------------------------------------------------------")
# Get into ActiveEngineeringState
issueRaiseM1M3Command()
checkSummaryStateEquals(RaisingEngineeringState, "Transition from ParkedEngineeringState to RaisingEngineeringState after RaiseM1M3")
waitForSummaryStateEquals(ActiveEngineeringState, 300, "Transition from RaisingEngineeringState to ActiveEngineeringState after RaisingEngineeringState completes")
# Verify Start command does nothing
issueStartCommand()
checkNoSummaryState("No transition from ActiveEngineeringState after Start command")
# Verify Enable command does nothing
issueEnableCommand()
checkNoSummaryState("No transition from ActiveEngineeringState after Enable command")
# Verify RaiseM1M3 command does nothing
issueRaiseM1M3Command()
checkNoSummaryState("No transition from ActiveEngineeringState after RaiseM1M3 command")
# Verify AbortRaiseM1M3 command does nothing
issueAbortRaiseM1M3Command()
checkNoSummaryState("No transition from ActiveEngineeringState after AbortRaiseM1M3 command")
# Verify EnterEngineering command does nothing
issueEnterEngineeringCommand()
checkNoSummaryState("No transition from ActiveEngineeringState after EnterEngineering command")
# Verify ExitEngineering command transitions to ActiveState
issueExitEngineeringCommand()
checkSummaryStateEquals(ActiveState, "Transition from ActiveEngineeringState to ActiveState after ExitEngineering command")
issueEnterEngineeringCommand()
checkSummaryStateEquals(ActiveEngineeringState, "Transition from ActiveState to ActiveEngineeringState after EnterEngineering command")
# Verify Disable command does nothing
issueDisableCommand()
checkNoSummaryState("No transition from ActiveEngineeringState after Disable command")
# Verify Standby command does nothing
issueStandbyCommand()
checkNoSummaryState("No transition from ActiveEngineeringState after Standby command")
# Verify Shutdown command does nothing
issueShutdownCommand()
checkNoSummaryState("No transition from ActiveEngineeringState after Shutdown command")
# Verify LowerM1M3 command transitions to LoweringEngineeringState
issueLowerM1M3Command()
checkSummaryStateEquals(LoweringEngineeringState, "Transition from ActiveEngineeringState to LoweringEngineeringState after LowerM1M3")
waitForSummaryStateEquals(ParkedEngineeringState, 300, "Transition from LoweringEngineeringState to ParkedEngineeringState after LoweringEngineeringState completes")
##################################  

##################################  
print("----------------------------------------------------------------------")
print("-- LoweringEngineeringState Verification")
print("----------------------------------------------------------------------")
# Get to LoweringEngineeringState
issueRaiseM1M3Command()
checkSummaryStateEquals(RaisingEngineeringState, "Transition from ParkedEngineeringState to RaisingEngineeringState after RaiseM1M3")
waitForSummaryStateEquals(ActiveEngineeringState, 300, "Transition from RaisingEngineeringState to ActiveEngineeringState after RaisingEngineeringState completes")
issueLowerM1M3Command()
checkSummaryStateEquals(LoweringEngineeringState, "Transition from ActiveEngineeringState to LoweringEngineeringState after LowerM1M3")
# Verify Start command does nothing
issueStartCommand()
checkNoSummaryState("No transition from LoweringEngineeringState after Start command")
# Verify Enable command does nothing
issueEnableCommand()
checkNoSummaryState("No transition from LoweringEngineeringState after Enable command")
# Verify RaiseM1M3 command does nothing
issueRaiseM1M3Command()
checkNoSummaryState("No transition from LoweringEngineeringState after RaiseM1M3 command")
# Verify AbortRaiseM1M3 command does nothing
issueAbortRaiseM1M3Command()
checkNoSummaryState("No transition from LoweringEngineeringState after AbortRaiseM1M3 command")
# Verify LowerM1M3 command does nothing
issueLowerM1M3Command()
checkNoSummaryState("No transition from LoweringEngineeringState after LowerM1M3 command")
# Verify EnterEngineering command does nothing
issueEnterEngineeringCommand()
checkNoSummaryState("No transition from LoweringEngineeringState after EnterEngineering command")
# Verify ExitEngineering command does nothing
issueExitEngineeringCommand()
checkNoSummaryState("No transition from LoweringEngineeringState after ExitEngineering command")
# Verify Disable command does nothing
issueDisableCommand()
checkNoSummaryState("No transition from LoweringEngineeringState after Disable command")
# Verify Standby command does nothing
issueStandbyCommand()
checkNoSummaryState("No transition from LoweringEngineeringState after Standby command")
# Verify Shutdown command does nothing
issueShutdownCommand()
checkNoSummaryState("No transition from LoweringEngineeringState after Shutdown command")
# Verify LowerM1M3 command completes
waitForSummaryStateEquals(ParkedEngineeringState, 300, "Transition from LoweringEngineeringState to ParkedEngineeringState after LoweringEngineeringState completes")
##################################  

issueDisableCommand()
issueStandbyCommand()

print("Cleaning up SAL")
time.sleep(1)
salM1M3.salShutdown()
exit()
