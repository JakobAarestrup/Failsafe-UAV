import logging
from datetime import datetime

# Python split() method example  
# Variable declaration  
RDSData = '76.0304, 26.0405, 9, 0.5, 20.0, 0.0, 10.0, 69.0420, 25.0420, 0.5, 20.0, 0.0, 10.0'  

# get current date and time
current_datetime = datetime.now().replace(microsecond=0)
date = datetime.strptime(sourceStamp.replace(' ', ''), '%M/%d/%y')
print("Current date & time : ", current_datetime)
  
# convert datetime obj to string
str_current_datetime = str(current_datetime)

# create a file object along with extension
file_name = "ServerLOG_" + str_current_datetime + ".txt"
file = open(file_name, 'w')

print("Log file created: ", file.name)
file.close()

# Logging the data
#logging.basicConfig(filename="log.txt")

# Splitting the values into variables
RDSLon, RDSLat, RDSSat, RDSAltitude, RDSRoll, RDSPitch, RDSYaw, SYSLon, SYSLat, SYSAlt, SYSRoll, SYSPitch, SYSYaw  = RDSData.split(", ",13) 

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