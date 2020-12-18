from torchvision import datasets, transforms
from sklearn.svm import SVC
from sklearn.preprocessing import LabelEncoder
from load_dataset import load_dataset
from MobileFacenet import MobileFaceNet

import torch.nn.functional
import torch
import pickle
import math
import os
import cv2
import numpy as np


model = MobileFaceNet(512).to(torch.device("cuda:0"))
model.load_state_dict(torch.load('../model.pth'))

dataset = load_dataset('../Processed/')
images = []
labels = []

transform = transforms.Compose([transforms.ToTensor(), transforms.Normalize([0.5, 0.5, 0.5], [0.5, 0.5, 0.5])])

for class_name in dataset:
  for path in class_name.paths:
    img = cv2.imread(path)
    img = cv2.resize(img,(112,112))
    img = transform(img)
    img = img.type(torch.FloatTensor)
    images.append(img)
    labels.append(class_name.name)

img_batch = torch.utils.data.DataLoader(images, batch_size=32)
labels = np.array(labels)
dataloader = torch.utils.data.DataLoader(dataset, batch_size=32, shuffle=True, num_workers=2)

#---------------------CREATE EMBEDDING AND LABEL-----------------------------------------
labels_encoder = LabelEncoder().fit(labels)
labelsNum = labels_encoder.transform(labels)
nClasses = len(labels_encoder.classes_)
nrof_img = len(labelsNum)
emb = np.zeros((nrof_img,512))
idx = 0

model.eval()

for batch in iter(img_batch):
  with torch.no_grad():
    batch = batch.to(torch.device("cuda:0"))
    embedding = model(batch).cpu()
  emb[idx:idx+32,:] = embedding
  idx += 32


f = open("../Data/test", "w")
for index in range(nrof_img):
  f.write(str(labelsNum[index]) + " ")
  for i in range(512):
    f.write(str(i) + ":" + str(float(np.round(emb[index][i], 5))) + " ")
  f.write("\n")

# img = cv2.imread("../test.jpg")
# img = cv2.resize(img,(112,112))
# img = transform(img)
# img = img.type(torch.FloatTensor)
# img = torch.unsqueeze(img, 0)

# model.eval()

# with torch.no_grad():
#   img = img.to(torch.device("cuda:0"))
#   embedding = model(img).cpu()
# print(embedding)