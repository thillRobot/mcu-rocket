#!/usr/bin/env python

# Data Acquisition Rocket - GSET - Tennessee Technological University
# Tristan Hill - June 24, 2021

#import some libraries
import csv
import numpy as np
import matplotlib.pyplot as plt

#set the local path to the data files
path = "launch-data/"

#open the file 
with open(path+'NANO-0-06-25-2025b.txt', newline='') as csvfile:
    # create file identifiers for managing the input and output files
    fid_in = csv.reader(csvfile, delimiter=',', quotechar='|')
    #fid_out = open(path+'NANO-1-06-24-2021-parsed.txt','w') #not using yet

    # initialize empty lists to append the data to
    row_index=[]
    time=[]        

    linear_acceleration_x=[]
    linear_acceleration_y=[]
    linear_acceleration_z=[]
   
    angular_position_x = []
    angular_position_y = []
    angular_position_z = []
    
    angular_velocity_x = []
    angular_velocity_y = []
    angular_velocity_z = []
    
    rotationvector_x=[]
    rotationvector_y=[]
    rotationvector_z=[]

    temperature=[]
    pressure=[]
    altitude=[]
    
    # iterate through the rows of data
    for i,row in enumerate(fid_in):
        #do stuff only if the row is not empty
        if len(row)>0:
           
            print(row, len(row)) 
            #print(str(row).split(",\'\""))
            name=row[0].split(':')[0]
            print(name)
            #items=str(row).split(",\'\" ")[0]
            #print(items, len(items))
            values=[row[0].split(':')[-1]]
            for i,item in enumerate(row[1:len(row)]):
                print(i,item)
                if len(item)>0:
                    values.append(item[0:len(item)-1])
            print(values) 
            if name=="Time":
                #row_index.append(row[0].split(':')[1].split(',')[0])
                #time.append(row[-1].split(' ')[0].split(';')[0])
                row_index.append(int(values[0]))
                time.append(float(values[1]))
            
            elif name=="LinAccel": 
                 linear_acceleration_x.append(float(values[0]))
                 linear_acceleration_y.append(float(values[1]))
                 linear_acceleration_z.append(float(values[2]))
            
            elif name=="AngPos":
                 angular_position_x.append(float(values[0]))
                 angular_position_y.append(float(values[1]))
                 angular_position_z.append(float(values[2])) 
            
            elif name=="AngVel":  # repeat for the other sensor types
                 angular_velocity_x.append(float(values[0]))
                 angular_velocity_y.append(float(values[1]))
                 angular_velocity_z.append(float(values[2])) 
 
            elif name=="Temp":  # repeat for the other sensor types
                 temperature.append(float(values[0]))
 
            elif name=="Pres":  # repeat for the other sensor types
                 pressure.append(float(values[0]))
            
            elif name=="Alt":  # repeat for the other sensor types
                 altitude.append(float(values[0]))

    # fid_out.close() # coming soon
   
    print("row_index:", row_index) 
    print("time: ", time)

    N=300 # choose an index to stop at 
    tstart=700
    tstop=len(time)-3
    t=np.arange(tstart,tstop) # create a range of values with numpy

    plt.figure(1) # setup a figure 1 window

    plt.subplot(411) # setup a subplot (#rows #cols currentindex)
    plt.plot(time[tstart:tstop],linear_acceleration_x[tstart:tstop],linestyle='',marker='.')
    plt.plot(time[tstart:tstop],linear_acceleration_y[tstart:tstop],linestyle='',marker='.')
    plt.plot(time[tstart:tstop],linear_acceleration_z[tstart:tstop],linestyle='',marker='.')
    plt.ylabel('Linear Acceleration')
    
    plt.subplot(412) # setup a subplot (#rows #cols currentindex)
    plt.plot(time[tstart:tstop],angular_position_x[tstart:tstop],linestyle='',marker='.')
    plt.plot(time[tstart:tstop],angular_position_y[tstart:tstop],linestyle='',marker='.')
    plt.plot(time[tstart:tstop],angular_position_z[tstart:tstop],linestyle='',marker='.')
    plt.ylabel('Angular Position')

    plt.subplot(413)
    plt.plot(time[tstart:tstop],angular_velocity_x[tstart:tstop],linestyle='',marker='.')
    plt.plot(time[tstart:tstop],angular_position_y[tstart:tstop],linestyle='',marker='.')
    plt.plot(time[tstart:tstop],angular_position_z[tstart:tstop],linestyle='',marker='.')
    plt.ylabel('Angular Velocity')

    plt.subplot(414)
    #plt.plot(time[tstart:tstop],temperature[tstart:tstop],linestyle='',marker='.')
    #plt.plot(time[tstart:tstop],pressure[tstart:tstop],linestyle='',marker='.')
    plt.plot(time[tstart:tstop],altitude[tstart:tstop],linestyle='',marker='.')
    plt.ylabel('Temp., Pres., Alt.')
    plt.show() # show the figure
