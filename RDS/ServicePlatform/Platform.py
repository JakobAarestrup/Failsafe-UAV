
import threading
import _thread
import os
import sys #Imports sys, used to end the program later
import tkinter
import time
import numpy as np
import math
from matplotlib import pyplot as plt
from tkinter import *
from tkinter import ttk
from functools import partial
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import (FigureCanvasTkAgg, 
NavigationToolbar2Tk)
#from tkintermapview import TkinterMapView
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from math import sin, cos, tan, asin, acos, atan2, fabs, sqrt

def angle2dcm(yaw, pitch, roll, input_units='rad', rotation_sequence='321'):
    # Apply necessary unit transformations.
    if input_units == 'rad':
        pass
    elif input_units == 'deg':
        yaw, pitch, roll = np.radians([yaw, pitch, roll])

    # Build transformation matrix Rnav2body.
    s_r, c_r = sin(roll) , cos(roll)
    s_p, c_p = sin(pitch), cos(pitch)
    s_y, c_y = sin(yaw)  , cos(yaw)

    if rotation_sequence == '321':
        # This is equivalent to Rnav2body = R(roll) * R(pitch) * R(yaw)
        # where R() is the single axis rotation matrix.  We implement
        # the expanded form for improved efficiency.
        Rnav2body = np.matrix([
             [c_y*c_p               ,  s_y*c_p              , -s_p    ],
             [-s_y*c_r + c_y*s_p*s_r,  c_y*c_r + s_y*s_p*s_r,  c_p*s_r],
             [ s_y*s_r + c_y*s_p*c_r, -c_y*s_r + s_y*s_p*c_r,  c_p*c_r]])

    else: 
        # No other rotation sequence is currently implemented
        print('WARNING (angle2dcm): requested rotation_sequence is unavailable.')
        print('                     NaN returned.')
        Rnav2body = np.nan

    return Rnav2body
                                               
def validateLogin():
    if username.get() == "Jakob" and password.get() == "PPWORLD": #Checks whether username and password are correct
        root.deiconify() #Unhides the root window
        login_window.destroy() #Removes the toplevel window         
    else:
        return
    
def positionUpdater(x,y):
    position = map_widget.set_position(x, y)
    marker = map_widget.set_marker(x,y)
    zoom = map_widget.set_zoom(10)
    
def Orientation3D(roll,pitch,yaw):

    A = [-0.5,-0.5,-0.5]
    B = [0.5,-0.5,-0.5]
    C = [-0.5,0.5,-0.5]
    D = [-0.5,-0.5,0.1]
    E = [-0.5,0.5,0.1]
    F = [0.5,-0.5,0.1]
    G = [0.5,0.5,-0.5]
    H = [0.5,0.5,0.1]

    BE = [1,-0.5,-0.5]
    CE = [-0.5,1,-0.5]
    DE = [-0.5,-0.5,1.1]

    RE = [A,BE]
    PE = [A,CE]
    YE = [A,DE]
    P = [A,B,F,H,G,C,A,D,E,H,F,D,E,C,G,B];
    
    
    fig = Figure(figsize = (4, 3.5), dpi =100)
    ax = fig.add_subplot(111, projection='3d')

    dcm = angle2dcm(yaw, pitch, roll,'deg')
    
    P = P*dcm
    RE = RE*dcm
    PE = PE*dcm
    YE = YE*dcm
    
    P11 = P[:,0]
    P21 = P[:,1]
    P31 = P[:,2]
    
    P11 = np.array(P11).ravel()
    P21 = np.array(P21).ravel()
    P31 = np.array(P31).ravel()
    
    
    RE11 = RE[:,0]
    RE21 = RE[:,1]
    RE31 = RE[:,2]
    
    RE11 = np.array(RE11).ravel()
    RE21 = np.array(RE21).ravel()
    RE31 = np.array(RE31).ravel()
    
    PE11 = PE[:,0]
    PE21 = PE[:,1]
    PE31 = PE[:,2]
    
    PE11 = np.array(PE11).ravel()
    PE21 = np.array(PE21).ravel()
    PE31 = np.array(PE31).ravel()
    
    
    YE11 = YE[:,0]
    YE21 = YE[:,1]
    YE31 = YE[:,2]
    
    YE11 = np.array(YE11).ravel()
    YE21 = np.array(YE21).ravel()
    YE31 = np.array(YE31).ravel()
    
    str_roll  = "Roll = " + str(roll) + "$^\circ$"
    str_Pitch = "Pitch = " + str(pitch) + "$^\circ$"
    str_Yaw   = "Yaw = " + str(yaw) + "$^\circ$"
    
    ax.plot(P11,P21,P31,'k')
    ax.plot(RE11,RE21,RE31,'b',linewidth=4)
    ax.plot(PE11,PE21,PE31,'r',linewidth=4)
    ax.plot(YE11,YE21,YE31,'g',linewidth=4)
    
    #adding text inside the plot
    ax.text(-0.2,-3,-4, str_roll, fontsize = 7, bbox = dict(facecolor = 'blue', alpha = 0.75))
    ax.text(0.9,-0.35,-2.35, str_Pitch, fontsize = 7, bbox = dict(facecolor = 'red', alpha = 0.75))
    ax.text(-4,-3,-5.875, str_Yaw, fontsize = 7, bbox = dict(facecolor = 'green', alpha = 0.75))
    
    #add legend
    ax.legend(['Drone','Roll','Pitch','Yaw'], fontsize=7, loc='upper right', frameon=False, ncol=4)
    
    #Turn off axis and limit axises
    ax.axis('off')
    ax.set_xlim(-1,1)
    ax.set_ylim(-1,1)
    ax.set_zlim(-1,1)
   
    #View adjusting
    ax.view_init(20,40)

    # Creating the Tkinter canvas containing the Matplotlib figure
    canvas = FigureCanvasTkAgg(fig,
                               master = root)  
    

    canvas.get_tk_widget().place(relx=0.5, rely=0.5, anchor = tkinter.SW)
        
    # update the plot 
    canvas.draw()
  
    
    
    return canvas
    
    
def plot_Height(y):
    
     
    fig2 = Figure(figsize = (5.55, 4.25)) #dpi =100
    # list of squares
    
  
    # adding the subplot
    plot1 = fig2.add_subplot(111)
  
    
    # Plot a rectangle with x, y, width, and height coordinates
    plot1.add_patch(plt.Rectangle((45,0), 10, y, color='blue'))

    # Set the x and y limits of the plot
    
    
    x_axis = [5]
    h = y
    
    height = [h]

    # Create the bar chart
    plot1.bar(x_axis, height)

    # Add labels to the x and y axes
    plot1.set_xlabel('RDS')
    plot1.set_label('Height')

    plot1.set_xlim(0, 10)
    plot1.set_ylim(0,320)
    plot1.set_xticks([], [])
    # creating the Tkinter canvas
    # containing the Matplotlib figure
    canvas = FigureCanvasTkAgg(fig2, master = root)  
    canvas.get_tk_widget().place(relx=0.5, rely=0.5, anchor = tkinter.NE)
    
    canvas.draw()
    
  
     
    
def terminalExecute():
    root.destroy() #Removes the hidden root window
    sys.exit() #Ends the script
    

# This flag will be used to stop the repeatable process
stop_flag = False

# This function will be the target of the thread
def live_logging():
    global stop_flag
    
    if not stop_flag:
        last_line = read_last_line(file_path)
        tokens = last_line.split()
        
        if tokens[0] == "Longitude:":
            longitude = float(tokens[1])
            latitude = float(tokens[4])
            satellites = float(tokens[7])
            
            
            altitude = float(tokens[9])
            roll = float(tokens[11])
            pitch = float(tokens[13])
            yaw = float(tokens[15])
            
            height_plot = plot_Height(altitude)
            Ori_plot = Orientation3D(roll,pitch,yaw)
            #positionUpdater(latitude,longitude)
            root.after(200, live_logging)
        

# This function will be called when the "Start" button is clicked
def stop_logging():
    global stop_flag, thread
    stop_flag = True
    # Stop the thread (assuming it's named "thread")
    thread.join()

def start_logging():
    global stop_flag, thread
    stop_flag = False
    # Start the thread (assuming it's named "thread")
    thread = threading.Thread(target=live_logging)
    thread.start()
    
def read_last_line(file_path):
    # Open the file in read mode
    with open(file_path, "r") as file:
        # Read all lines in the file
        lines = file.readlines()
        # Return the last line in the file
        last_line = lines[-2]  
        text.insert(END,last_line)
        return last_line



#Main window
root = tkinter.Tk()
root.geometry(f"{800}x{660}")
root.title("RDS Terminal.py")

# =============================================================================
# # create map widget
# map_widget = TkinterMapView(root, width=400, height=300, corner_radius=0)
# map_widget.place(relx=0.5, rely=0.5, anchor=tkinter.NW)
#   
# # google normal tile server
# map_widget.set_tile_server("https://mt0.google.com/vt/lyrs=m&hl=en&x={x}&y={y}&z={z}&s=Ga", max_zoom=15)
# # set current widget position and zoom
# map_widget.set_position(55, 9.5)  # Danmark
# map_widget.set_marker(55, 9.5)
# 
# =============================================================================

text=Text(root, width=50, height=18.4)

text.grid(row = 0, column = 2)


#Login window
login_window = Toplevel()
login_window.geometry('250x100')
login_window.title('RDS Login Side')


#username label and text entry box
usernameLabel = Label(login_window, text="Username").grid(row=0, column=0)
username = StringVar()
usernameEntry = Entry(login_window, width = 20, borderwidth = 1,textvariable=username).grid(row=0, column=1)

#usernameEntry = Entry(service_window, width = 20, borderwidth = 1,textvariable=username).grid(row=0, column=1)

#password label and password entry box
passwordLabel = Label(login_window,text="Password").grid(row=1, column=0)
password = StringVar()
passwordEntry = Entry(login_window, textvariable=password, show='*').grid(row=1, column=1)


#login button
loginButton = Button(login_window, text="Login", command=validateLogin).grid(row=4, column=1)



# in main window
#button3.place(x=320,y=0)


start_logging = Button(root ,command=start_logging, height = 2, width = 10, text="Live Logging", )
start_logging.place(x=80,y=290)

stop_logging = Button(root, command=stop_logging, height = 2, width = 10, text="Stop Logging")
stop_logging.place(x=160,y=290)

# button that displays the plot
exit_window = Button(master = root, command = terminalExecute, height = 2, width = 10, text = "EXIT")
exit_window.place(x=240,y=290)

# Get a list of all files in the specified folder
file_path = "C:/Users/Rasmus/Documents/GUI/Logfile.txt" 
    

roll = 0
pitch = 0
yaw = 0


plot_Height(0)


Orientation3D(roll,pitch,yaw)

root.withdraw()
root.mainloop()

    