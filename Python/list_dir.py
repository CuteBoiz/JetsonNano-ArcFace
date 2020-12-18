from load_dataset import load_dataset
from sklearn.preprocessing import LabelEncoder

import pickle
import math
import os
import cv2
import numpy as np

dataset = load_dataset('../Processed/')

f_data = open("../Data/data.txt", "w")
f_label = open("../Data/label.txt", "w")
f_inversve_label = open("../Data/inverse_label.txt", "w")

labels = []

for class_name in dataset:
  for path in class_name.paths:
  	f_data.write(str(path) + "\n")
  	labels.append(class_name.name)

labels = np.array(labels)
labels_encoder = LabelEncoder().fit(labels)

nClasses = len(labels_encoder.classes_)
labelsNum = labels_encoder.transform(labels)

for label in labelsNum:
	f_label.write(str(label) + "\n")

for i in range(nClasses):
	f_inversve_label.write(str(i) + "\t" + str(labels_encoder.inverse_transform([i])).replace("[", "").replace("'", "").replace("]", "") + "\n")
