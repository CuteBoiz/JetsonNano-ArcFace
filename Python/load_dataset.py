import os
import cv2
import numpy as np
import time
import warnings

class ImageClass:
	def __init__(self, name, paths):
		assert name is not None
		assert paths is not None

		self.name = name
		self.paths = paths

def get_image_paths(path):
	image_paths = []
	if os.path.isdir(path):
		images = os.listdir(path)
		for img in images:
			if os.path.isfile(os.path.join(path, img)):
				image_paths.append(os.path.join(path, img))
	return image_paths


def load_dataset(path):
	dataset = []
	path_exp = os.path.expanduser(path)
	classes = [path for path in os.listdir(path_exp) \
					if os.path.isdir(os.path.join(path_exp, path))]
	classes.sort()
	nrof_classes = len(classes)
	for i in range(nrof_classes):
		class_name = classes[i]
		facedir = os.path.join(path_exp, class_name)
		image_paths = get_image_paths(facedir)
		if len(image_paths) > 0:
			dataset.append(ImageClass(class_name, image_paths))
	return dataset

def load_people_per_batch(dataset, people_per_batch, images_per_person):
	nrof_classes = len(dataset)
	class_indices = np.arange(nrof_classes)
	np.random.shuffle(class_indices)

	image_paths = []
	num_per_class = []

	for i in range(min(people_per_batch, nrof_classes)):
		class_index = class_indices[i]
		nrof_images_in_class = len(dataset[class_index].paths)
		
		nrof_example_images = min(images_per_person, nrof_images_in_class)
		num_per_class.append(nrof_example_images)
		
		image_indices = np.arange(nrof_images_in_class)
		np.random.shuffle(image_indices)
		idxs = image_indices[0:nrof_example_images]
		sampled_image = [dataset[class_index].paths[j] for j in idxs]
		image_paths += sampled_image

	return image_paths, num_per_class






