import socket
import logging
from datetime import datetime

localIP = ""
localPort = 42069
bufferSize = 1024
msgFromServer = "Message received from RDS"
bytesToSend = str.encode(msgFromServer)

# Create a datagram socket
UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

# Bind to address and ip
UDPServerSocket.bind((localIP, localPort))

print("UDP server up and listening")

# get current date and time
now = datetime.now()
current_time = now.strftime("%Y-%m-%d-%H")
current_clock = now.strftime("%H:%M:%S")
  
# convert datetime obj to string
str_current_datetime = str(current_time)

# create a file object along with extension
file_name = "C:/Users/Rasmus/Documents/GUI/Logs/ServerLOG_" + str_current_datetime + ".txt"

# Listen for incoming datagrams
while (True):
    bytesAddressPair = UDPServerSocket.recvfrom(bufferSize)
    message = bytesAddressPair[0].decode()
    address = bytesAddressPair[1]
    #print(message.decode())

    clientMsg = "{}, ".format(message)
    clientIP = "Client IP Address:{}".format(address)
    file = open(file_name, 'w')
    current_clock = now.strftime("%H:%M:%S")
    file.write(current_clock + clientMsg)
    file.close()
    print("Log file created: ", file.name)
    #print(clientIP)

    # Sending a reply to client
    UDPServerSocket.sendto(bytesToSend, address)