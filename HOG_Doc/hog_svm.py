#To make the predictions

import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import cv2

data = pd.read_csv('data.csv')
hog = cv2.HOGDescriptor()

X = data.iloc[:,1:3781]
y = data.iloc[:,3781]

image_indices = ['0','1','2','3','29','33','50','523','532','533','541','581','5747','5781','5786','5803','5807','5811','5819','5823','5833','5927','5961','5894','5904','6041','5970']

from sklearn import svm

clf = svm.SVC(kernel = 'linear')
clf.fit(X,y)

for ind in image_indices:
	im = cv2.imread("Images_HOG/Stripes/" + str(ind) + ".png")
	img = cv2.resize(im,(64,128))

	h = hog.compute(img)

	t = np.array(h)
	print(t.shape)
	y_ = clf.predict(np.transpose(t))

	print(ind + ": " + str(y_))