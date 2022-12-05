import logging
import datetime

# Python split() method example  
# Variable declaration  
str = '76.0304, 26.0405, 9, 0.5, 20.0, 0.0, 10.0, 69.0420, 25.0420, 0.5, 20.0, 0.0, 10.0'  

# Creating log file
#f = open('ServicePlatform_log.txt','w') as f # path_to_file = path on server side to place log file, mode = mode

def timeStamped(fname, fmt='%d-%m-%Y-%H-%M-%S_{ServicePlatform_log.txt}'):
    return datetime.datetime.now().strftime(fmt).format(fname=fname)

with open(timeStamped('ServicePlatform_log.txt'),'w') as outf:
    outf.write('data!')

# Logging the data
logging.basicConfig(filename="log.txt")

# Splitting the values into variables
RDSLon, RDSLat, RDSSat, RDSAltitude, RDSRoll, RDSPitch, RDSYaw, SYSLon, SYSLat, SYSAlt, SYSRoll, SYSPitch, SYSYaw  = str.split(", ",13) 





# Displaying the data
print ("1. " + RDSLon)
print ("2. " + RDSLat)
print ("3. " + RDSSat)
print ("4. " + RDSAltitude)
print ("5. " + RDSRoll)
print ("6. " + RDSPitch)
print ("7. " + RDSYaw)
print ("8. " + SYSLon)
print ("9. " + SYSLat)
print ("10. " + SYSAlt)
print ("11. " + RDSRoll)
print ("12. " + RDSPitch)
print ("13. " + RDSYaw)