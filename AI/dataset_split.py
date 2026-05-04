import os
import random
import numpy as np

dataset_path = "../smart_greenhouse/azolla_augmented_dataset"
train_ratio = 0.7
val_ratio = 0.15
test_ratio = 0.15

images =os.listdir(dataset_path)
random.shuffle(images)

total_images=len(images)
train_end=int(total_images*train_ratio)
val_end=int(total_images*(train_ratio+val_ratio))

train_paths = [os.path.join(dataset_path, img) for img in images[:train_end]]
val_paths = [os.path.join(dataset_path, img) for img in images[train_end:val_end]]
test_paths = [os.path.join(dataset_path, img) for img in images[val_end:]]


os.makedirs("../smart_greenhouse/azolla_split_dataset/paths", exist_ok=True)

np.save("../smart_greenhouse/azolla_split_dataset/paths/train_paths.npy", np.array(train_paths))
np.save("../smart_greenhouse/azolla_split_dataset/paths/val_paths.npy", np.array(val_paths))
np.save("../smart_greenhouse/azolla_split_dataset/paths/test_paths.npy", np.array(test_paths))