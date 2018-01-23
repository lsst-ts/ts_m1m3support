import time
import sys
import numpy
from SALPY_m1m3 import *
import ILCSimulator
import InclinometerSimulator
import DisplaceSimulator
import AccelSimulator
import UDP

print("Starting SAL")
salM1M3 = SAL_m1m3()
salM1M3.setDebugLevel(0)

print("Initializing topics")
startTime = time.time()
salM1M3.salTelemetrySub("m1m3_InclinometerData")
endTime = time.time()

print("Initialization complete, took %0.3f seconds" % (endTime - startTime))

sampleCount = 0;
while sampleCount < 100:
    data = m1m3_InclinometerDataC()
    retVal = salM1M3.getSample_InclinometerData(data)
    if retVal == 0:
        sampleCount += 1
        print("%0.6f" % (data.Timestamp - 1510000000))
        time.sleep(0.190)
    else:
        time.sleep(0.001)

print("Cleaning up SAL")
time.sleep(1)
salM1M3.salShutdown()
exit()
