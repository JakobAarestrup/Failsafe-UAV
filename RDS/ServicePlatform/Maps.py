import tkinter
import time
from tkintermapview import TkinterMapView
from random import randint


root_tk = tkinter.Tk()
root_tk.geometry(f"{600}x{400}")
root_tk.title("map_view_simple_example.py")
 
# create map widget
map_widget = TkinterMapView(root_tk, width=1000, height=500, corner_radius=0)
map_widget.pack(fill="both", expand=True)
 
# google normal tile server
map_widget.set_tile_server("https://mt0.google.com/vt/lyrs=m&hl=en&x={x}&y={y}&z={z}&s=Ga", max_zoom=22)
# set current widget position and zoom
map_widget.set_position(55, 9.5)  # Paris, France


def positionupdater():
    # x = 54
    # y = 9
    x = randint(54,56)
    y = randint(9,10)
    position = map_widget.set_position(x, y)
    marker = map_widget.set_marker(x,y)
    zoom = map_widget.set_zoom(10)
    # position = map_widget.set_position(x+k*1, y)
    # marker = map_widget.set_marker(x+k*1,y)
    # zoom = map_widget.set_zoom(10)
    # position = map_widget.set_position(x+k*2, y)
    # marker = map_widget.set_marker(x+k*2,y)
    # zoom = map_widget.set_zoom(10)
   # marker_de = marker
    # print(marker)
   # root_tk.after(1000, positiondelete(marker))

positionupdater()

#def positiondelete(marker):
   # marker.delete()
    #positionupdater()
    
root_tk.mainloop()

#https://github.com/TomSchimansky/TkinterMapView