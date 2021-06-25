#!/usr/bin/env python

import csv

import matplotlib.pyplot as plt

path = "../launch-data/"

with open(path+'NANO-1-06-24-2021.txt', newline='') as csvfile:
	fid_in = csv.reader(csvfile, delimiter=' ', quotechar='|')

	fid_out = open(path+'NANO-1-06-24-2021-parsed.txt','w')

	orientation = []

	for i,row in enumerate(fid_in):

		if len(row)>0:
			items=str(row[0]).split(":")
			print(items)

			if items[0]=="Orientation":
				print(items[1])

				orientation.append(items[1])

		#print("index:"+str(i)+" data:"+str(data))

	fid_out.close()

	len(orientation)

	#lt.plot(orientation)
	#lt.ylabel('0ientation')
	#lt.show()