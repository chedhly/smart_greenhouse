import cv2 as cv
import numpy as np
from tensorflow.keras.utils import Sequence # type: ignore
from image_treatment import ImageTreatment


class PlantDataset(Sequence):

    def __init__(self, split="train", batch_size=32):

        self.batch_size = batch_size

        self.processor = ImageTreatment()

        self.image_paths = np.load(f"../smart_greenhouse/split_dataset/paths/{split}_paths.npy", allow_pickle=True)
        self.labels = np.load(f"../smart_greenhouse/split_dataset/labels/{split}_labels.npy", allow_pickle=True)

        self.class_names = np.load("../smart_greenhouse/class_names.npy", allow_pickle=True)

#return the number of batches per epoch

    def __len__(self):

        return int(np.ceil(len(self.image_paths) / self.batch_size))

#return a batch of images and labels given an index

    def __getitem__(self, index):

        batch_paths = self.image_paths[
            index*self.batch_size:(index+1)*self.batch_size
        ]

        batch_labels = self.labels[
            index*self.batch_size:(index+1)*self.batch_size
        ]

        images = [self.processor.preprocess(cv.imread(path)) for path in batch_paths]

        return np.array(images), np.array(batch_labels)
    def on_epoch_end(self):

        # Shuffle the dataset at the end of each epoch

        indices = np.arange(len(self.image_paths))

        np.random.shuffle(indices)

        self.image_paths = self.image_paths[indices]

        self.labels = self.labels[indices]