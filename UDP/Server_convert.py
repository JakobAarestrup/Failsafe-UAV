import logging
from datetime import datetime

# Python split() method example  
# Variable declaration  
RDSData = '76.0304, 26.0405, 9, 0.5, 20.0, 0.0, 10.0, 69.0420, 25.0420, 0.5, 20.0, 0.0, 10.0'  

# get current date and time
now = datetime.now()
current_time = now.strftime("%Y-%m-%d-%H%M")
current_clock = now.strftime("%H:%M:%S")
  
# convert datetime obj to string
str_current_datetime = str(current_time)

# create a file object along with extension
file_name = "ServerLOG_" + str_current_datetime + ".txt"
file = open(file_name, 'w')

print("Log file created: ", file.name)

# Splitting the values into variables
RDSLon, RDSLat, RDSSat, RDSAlt, RDSRoll, RDSPitch, RDSYaw, SYSLon, SYSLat, SYSAlt, SYSRoll, SYSPitch, SYSYaw  = RDSData.split(", ",13) 

# Logging the data
file.write(current_clock + ': ')
file.write('Longitude: ' + RDSLon + ", ")
file.write('Latitude: ' + RDSLat + ", ")
file.write('Satellites: ' + RDSSat + ", ")
file.write('Altitude: ' + RDSAlt + ", ")
file.write('Roll: ' + RDSRoll + ", ")
file.write('Pitch: ' + RDSPitch + ", ")
file.write('Yaw: ' + RDSYaw + ", ")
file.write('SYS_Longitude: ' + SYSLon + ", ")
file.write('SYS_Latitude: ' + SYSLat + ", ")
file.write('SYS_Altitude: ' + SYSAlt + ", ")
file.write('SYS_Roll: ' + SYSRoll + ", ")
file.write('SYS_Pitch: ' + SYSPitch + ", ")
file.write('SYS_Yaw: ' + SYSYaw + "\n")
file.close()