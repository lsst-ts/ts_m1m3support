import time
import sys
import numpy
from SALPY_m1m3 import *

print("Starting SAL")
salM1M3 = SAL_m1m3()
salM1M3.setDebugLevel(0)

print("Initializing topics")
startTime = time.time()
salM1M3.salTelemetrySub("m1m3_logevent_SummaryState")
salM1M3.salCommand("m1m3_command_Start")
salM1M3.salCommand("m1m3_command_Standby")
endTime = time.time()

print("Initialization complete, took %0.3f seconds" % (endTime - startTime))

# Issue start command
print("Issuing Start Command")
data = m1m3_command_StartC()
data.Start = True
data.SettingsToApply = "Default"
cmdId = salM1M3.issueCommand_Start(data)
salM1M3.waitForCompletion_Start(cmdId, 10)

# Check summary state
data = m1m3_logevent_SummaryStateC()
retVal = salM1M3.getEvent_SummaryState(data)
while True:
    if retVal == 0:
        break
    time.sleep(1)
    retVal = salM1M3.getEvent_SummaryState(data)
print("SummaryState: 0x%08X" % data.SummaryState)
    
# Issue standby command
print("Issuing Standby Command")
data = m1m3_command_StandbyC()
data.Standby = True
cmdId = salM1M3.issueCommand_Standby(data)
salM1M3.waitForCompletion_Standby(cmdId, 10)

# Check summary state
data = m1m3_logevent_SummaryStateC()
retVal = salM1M3.getEvent_SummaryState(data)
while True:
    if retVal == 0:
        break
    time.sleep(1)
    retVal = salM1M3.getEvent_SummaryState(data)
print("SummaryState: 0x%08X" % data.SummaryState)

print("Cleaning up SAL")
time.sleep(1)
salM1M3.salShutdown()
exit()
