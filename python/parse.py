#!/usr/bin/env python

import csv

import numpy as np

import matplotlib.pyplot as plt

path = "../launch-data/"

with open(path+'NANO-1-06-24-2021.txt', newline='') as csvfile:
	fid_in = csv.reader(csvfile, delimiter=' ', quotechar='|')

	fid_out = open(path+'NANO-1-06-24-2021-parsed.txt','w')

	orientation_x = []
	orientation_y = []
	orientation_z = []
	
	rotationvector_x=[]
	rotationvector_y=[]
	rotationvector_z=[]

	linearacceleration_x=[]
	linearacceleration_y=[]
	linearacceleration_z=[]


	for i,row in enumerate(fid_in):

		if len(row)>0:
			items=str(row[0]).split(":")
			
			if items[0]=="Orientation":
				values=items[1].split(",")
				orientation_x.append(float(values[0]))
				orientation_y.append(float(values[1]))
				orientation_z.append(float(values[2]))

			elif items[0]=="RotationVector":
				values=items[1].split(",")
				rotationvector_x.append(float(values[0]))
				rotationvector_y.append(float(values[1]))
				rotationvector_z.append(float(values[2]))

			elif items[0]=="LinearAcceleration":
				values=items[1].split(",")
				linearacceleration_x.append(float(values[0]))
				linearacceleration_y.append(float(values[1]))
				linearacceleration_z.append(float(values[2]))

	fid_out.close()

	N=2500
	t=np.arange(0,N)

	plt.figure(1)

	plt.subplot(311)
	plt.plot(t,orientation_x[0:N])
	plt.plot(t,orientation_y[0:N])
	plt.plot(t,orientation_z[0:N])
	plt.ylabel('Orientation')

	plt.subplot(312)
	plt.plot(t,rotationvector_x[0:N])
	plt.plot(t,rotationvector_y[0:N])
	plt.plot(t,rotationvector_z[0:N])
	plt.ylabel('RotationVector')

	plt.subplot(313)
	plt.plot(t,linearacceleration_x[0:N])
	plt.plot(t,linearacceleration_y[0:N])
	plt.plot(t,linearacceleration_z[0:N])
	plt.ylabel('LinearAcceleration')

	plt.show()
