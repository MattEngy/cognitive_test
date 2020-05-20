import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl
import getopt
import seaborn as sns
import sys
import subprocess, os
import time

def get_colour_by_id(id_inp): #just generating some distributed colours dy id
        return ((id_inp * 45 + 128) % 255) / 255, ((id_inp * 25 + 64) % 255) / 255, ((id_inp * 10 + 32) % 255) / 255
        

type_to_point_size = {
    'Pedestrian': 5,
    'Vehicle': 50,
    'TrafficLight': 100
}
file_name = '..\\trm.169.049.det.tsv.result'

exe_file_path = '..\\Debug\\cognitive_test.exe'

#for integration with exe file in future
#calcProc=subprocess.Popen([exe_file_path])
#calcProc.wait()

#arrays to store coloumns from file
msec = []
x = []
y = []

#coloumn indexes
msec_index = 0
type_index = 1
x_index = 2
y_index = 3
id_index = 4

sns.set_style("white")
sns.set_style("ticks")
sns.set_context("notebook", font_scale=1)

lines = []
file_cur = open(file_name, 'r')
lines = file_cur.readlines()
file_cur.close()

plt.grid(c='gray', ls='--', lw=0.5)
plt.xlabel('X, meters')
plt.ylabel('Y, meters')
plt.title('Object trajectories, small black dots - filtered\nthin coloured - Pedestrians\nthicker ones - Vehicles\neven thicker - Trafficlights')
plt.ion()#allows dynamic plotting
msec_cur = 0
#points_to_plot = [,]
for k in range(1, len(lines)): #skipping first line with col names
        S_cur = lines[k].split('\t')
        #getting values from S_cur
        id_cur = int(S_cur[id_index])
        type_cur = S_cur[type_index]
        x_cur = float(S_cur[x_index])
        y_cur = float(S_cur[y_index])
#	msec.append(int(S_cur[0]))	
#	x.append(float(S_cur[1]))
#	y.append(float(S_cur[2]))
        if (id_cur > 0) :
                plt.scatter(x_cur, y_cur, s = type_to_point_size[type_cur], color = get_colour_by_id(id_cur))
        else :
                plt.scatter(x_cur, y_cur, s = 1, color = 'k')
#                plt.pause(0.005)
        if (msec_cur > int(S_cur[msec_index])) :
                msec_cur = S_cur[msec_index]
#                plt.pause(0.2)
print('DONE!')
plt.show()
#plt.close()
