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
    fid_in = csv.reader(csvfile, delimiter=' ', quotechar='|')
    #fid_out = open(path+'NANO-1-06-24-2021-parsed.txt','w') #not using yet

    # initialize empty lists to append the data to
    row_index=[]
    time=[]        

    orientation_x = []
    orientation_y = []
    orientation_z = []
    
    rotationvector_x=[]
    rotationvector_y=[]
    rotationvector_z=[]

    linearacceleration_x=[]
    linearacceleration_y=[]
    linearacceleration_z=[]

    # iterate through the rows of data
    for i,row in enumerate(fid_in):
        #do stuff only if the row is not empty
        if len(row)>0:
            
            print(str(row).split(",\'\""))
            name=row[0].split(':')[0]
            print(name)
            items=str(row).split(",\'\"")[0].split(',')
            print(type(items))
            if name=="Time":
                
                row_index.append(row[0].split(':')[1].split(',')[0])
                time.append(row[-1].split(' ')[0].split(';')[0])
                #time.append(row[1].split('')[-1])          
        #    if items[0]=="LinAccel":
        #        # split the three values delimited by ","
        #        #print(row)
        #        values=items[1].split(",")
        #        #print(items)
        #        # append the values as floats the the lists we initialized
        #        orientation_x.append(float(values[0]))
        #        orientation_y.append(float(values[1]))
        #        orientation_z.append(float(values[2])) 
        #            
        #   elif items[0]=="AngVel":  # repeat for the other sensor types
        #       values=items[1].split(",")
        #       rotationvector_x.append(float(values[0]))
        #       rotationvector_y.append(float(values[1]))
        #       rotationvector_z.append(float(values[2]))

        #   elif items[0]=="AngPos": 
        #       values=items[1].split(",")
        #       linearacceleration_x.append(float(values[0]))
        #       linearacceleration_y.append(float(values[1]))
        #       linearacceleration_z.append(float(values[2]))
        
    # fid_out.close() # coming soon
   
    print("row_index:", row_index) 
    print("time: ", time)


    N=300 # choose an index to stop at 
    tstart=2150
    tstop=tstart+N
    t=np.arange(tstart,tstop) # create a range of values with numpy

    plt.figure(1) # setup a figure 1 window

    plt.subplot(311) # setup a subplot (#rows #cols currentindex)
    plt.plot(t,orientation_x[tstart:tstop])
    plt.plot(t,orientation_y[tstart:tstop])
    plt.plot(t,orientation_z[tstart:tstop])
    plt.ylabel('LinAccel')

    plt.subplot(312)
    plt.plot(t,rotationvector_x[tstart:tstop])
    plt.plot(t,rotationvector_y[tstart:tstop])
    plt.plot(t,rotationvector_z[tstart:tstop])
    plt.ylabel('AngVel')

    plt.subplot(313)
    plt.plot(t,linearacceleration_x[tstart:tstop])
    plt.plot(t,linearacceleration_y[tstart:tstop])
    plt.plot(t,linearacceleration_z[tstart:tstop])
    plt.ylabel('AngPos')

    plt.show() # show the figure
