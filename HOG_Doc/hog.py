#To create the training data to a csv file


import cv2 
import pandas as pd 
import numpy as np 

hog = cv2.HOGDescriptor()

file = open("data.txt","w+")

#
im = cv2.imread("Images_HOG/Obstacles/0.png")
img = cv2.resize(im,(64,128))

h = hog.compute(img)
t = np.array(h)
print(t.shape)
prediction = 1

file.write(str(prediction) + " ")
l = 1
for a in range(len(t)):
	s = str(t[a])
	s = s.replace('[','')
	s = s.replace(']','')
	file.write(str(l) + ":" + s + " ")
	l+=1
file.write("\n")

t = np.insert(t,len(t),prediction)
data = np.array(t)
#

for i in range(1,84):
	im = cv2.imread("Images_HOG/Obstacles/" + str(i) + ".png")
	cv2.imshow('a',im)
	#cv2.waitKey(1)
	img = cv2.resize(im,(64,128))

	h = hog.compute(img)
	t = np.array(h)
	prediction = 1

	l = 1
	file.write(str(prediction) + " ")
	for a in range(len(t)):
		s = str(t[a])
		s = s.replace('[','')
		s = s.replace(']','')
		file.write(str(l) + ":" + s + " ")
		l+=1
	file.write("\n")

	t = np.insert(t,len(t),prediction)
	data = np.vstack((data,t))

for i in range(0,84):
	im = cv2.imread("Images_HOG/Non-obstacles/" + str(i) + ".png")
	cv2.imshow('a',im)
	#cv2.waitKey(1)
	img = cv2.resize(im,(64,128))

	h = hog.compute(img)
	t = np.array(h)
	prediction = 0

	file.write(str(prediction) + " ")

	l = 1
	for a in range(len(t)):
		s = str(t[a])
		s = s.replace('[','')
		s = s.replace(']','')
		file.write(str(l) + ":" + s + " ")
		l+=1
	file.write("\n")

	t = np.insert(t,len(t),prediction)
	data = np.vstack((data,t))


data_csv = pd.DataFrame(data)

data_csv.to_csv("data.csv")

