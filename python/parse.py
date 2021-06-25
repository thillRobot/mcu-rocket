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
with open(path+'NANO-1-06-24-2021.txt', newline='') as csvfile:
	# create file identifiers for managing the input and output files
	fid_in = csv.reader(csvfile, delimiter=' ', quotechar='|')
	#fid_out = open(path+'NANO-1-06-24-2021-parsed.txt','w') #not using yet

	# initialize empty lists to append the data to
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
			# split the names from the values delimited by ":"
			items=str(row[0]).split(":") 
			
			if items[0]=="Orientation":
				# split the three values delimited by ","
				values=items[1].split(",")
				# append the values as floats the the lists we initialized
				orientation_x.append(float(values[0]))
				orientation_y.append(float(values[1]))
				orientation_z.append(float(values[2]))	
				
			elif items[0]=="RotationVector":  # repeat for the other sensor types
				values=items[1].split(",")
				rotationvector_x.append(float(values[0]))
				rotationvector_y.append(float(values[1]))
				rotationvector_z.append(float(values[2]))

			elif items[0]=="LinearAcceleration": 
				values=items[1].split(",")
				linearacceleration_x.append(float(values[0]))
				linearacceleration_y.append(float(values[1]))
				linearacceleration_z.append(float(values[2]))

	# fid_out.close() # coming soon
	N=300 # choose an index to stop at 
	tstart=2150
	tstop=tstart+N
	t=np.arange(tstart,tstop) # create a range of values with numpy

	plt.figure(1) # setup a figure 1 window

	plt.subplot(311) # setup a subplot (#rows #cols currentindex)
	plt.plot(t,orientation_x[tstart:tstop])
	plt.plot(t,orientation_y[tstart:tstop])
	plt.plot(t,orientation_z[tstart:tstop])
	plt.ylabel('Orientation')

	plt.subplot(312)
	plt.plot(t,rotationvector_x[tstart:tstop])
	plt.plot(t,rotationvector_y[tstart:tstop])
	plt.plot(t,rotationvector_z[tstart:tstop])
	plt.ylabel('RotationVector')

	plt.subplot(313)
	plt.plot(t,linearacceleration_x[tstart:tstop])
	plt.plot(t,linearacceleration_y[tstart:tstop])
	plt.plot(t,linearacceleration_z[tstart:tstop])
	plt.ylabel('LinearAcceleration')

	plt.show() # show the figure
