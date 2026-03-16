import os
import random
import numpy as np

dataset_path = "../smart_greenhouse/dataset/augmented_plantvillage"
train_ratio = 0.7
val_ratio = 0.15
test_ratio = 0.15

train_paths,train_labels = [],[]
val_paths,val_labels = [],[]
test_paths,test_labels = [],[]

classes = sorted(os.listdir(dataset_path))

for idx, class_name in enumerate(classes):
    class_folder=os.path.join(dataset_path, class_name)
    images=os.listdir(class_folder)
    random.shuffle(images)

    total_images=len(images)
    train_end=int(total_images*train_ratio)
    val_end=int(total_images*(train_ratio+val_ratio))
    train_images=images[:train_end]
    val_images=images[train_end:val_end]
    test_images=images[val_end:]

    for img in train_images:
        train_paths.append(os.path.join(class_folder, img))
        train_labels.append(idx)
    for img in val_images:
        val_paths.append(os.path.join(class_folder, img))
        val_labels.append(idx)
    for img in test_images:
        test_paths.append(os.path.join(class_folder, img))
        test_labels.append(idx)

os.makedirs("../smart_greenhouse/split_dataset/paths", exist_ok=True)
os.makedirs("../smart_greenhouse/split_dataset/labels", exist_ok=True)
np.save("../smart_greenhouse/split_dataset/paths/train_paths.npy", np.array(train_paths))
np.save("../smart_greenhouse/split_dataset/labels/train_labels.npy", np.array(train_labels))
np.save("../smart_greenhouse/split_dataset/paths/val_paths.npy", np.array(val_paths))
np.save("../smart_greenhouse/split_dataset/labels/val_labels.npy", np.array(val_labels))
np.save("../smart_greenhouse/split_dataset/paths/test_paths.npy", np.array(test_paths))
np.save("../smart_greenhouse/split_dataset/labels/test_labels.npy", np.array(test_labels))