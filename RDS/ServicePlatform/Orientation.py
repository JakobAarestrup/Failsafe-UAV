import numpy as np
import math
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
from math import sin, cos, tan, asin, acos, atan2, fabs, sqrt
#https://gist.github.com/mike-koch/f405d307d01b96496e6f847cc8b88728

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

plt.style.use('seaborn-poster')

A = [0,0,0]
B = [1.3,0,0]
C = [0,1.3,0]
D = [0,0,0.3]
E = [0,1.3,0.3]
F = [1.3,0,0.3]
G = [1.3,1.3,0]
H = [1.3,1.3,0.3]

P = [A,B,F,H,G,C,A,D,E,H,F,D,E,C,G,B];


fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

roll = 30
pitch = 0
yaw = 0

dcm = angle2dcm(yaw, pitch, roll,'deg')

P = P*dcm

P11 = P[:,0]
P21 = P[:,1]
P31 = P[:,2]

P11 = np.array(P11).ravel()
P21 = np.array(P21).ravel()
P31 = np.array(P31).ravel()

ax.plot(P11,P21,P31)
#ax = fig.add_subplot(111, projection='3d')
#ax.add_collection3d(Poly3DCollection(P, facecolors='cyan', linewidths=1, edgecolors='k', alpha=.90))


ax.set_xlim(-1,2)
ax.set_ylim(-1,2)
ax.set_zlim(-1,2)
# it can be used to change the axes view
ax.view_init(20,40)


#plt.show