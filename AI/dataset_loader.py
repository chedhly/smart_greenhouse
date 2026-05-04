import os
import cv2 as cv
import numpy as np
from tensorflow.keras.utils import Sequence # type: ignore
from image_treatment import ImageTreatment


class PlantDataset(Sequence):

    def __init__(self, split="train", batch_size=32):

        self.batch_size = batch_size

        self.processor = ImageTreatment()

        self.image_paths = np.load(f"../smart_greenhouse/azolla_split_dataset/paths/{split}_paths.npy", allow_pickle=True)
       


#return the number of batches per epoch

    def __len__(self):

        return int(np.ceil(len(self.image_paths) / self.batch_size))

#return a batch of images and labels given an index

    def __getitem__(self, index):

        batch_paths = self.image_paths[
            index*self.batch_size:(index+1)*self.batch_size
        ]


        images = []
        for path in batch_paths:

            img = cv.imread(path)
            if img is None:
                continue
            img = self.processor.preprocess_image(img)

            images.append(img)

        return np.array(images)
    def on_epoch_end(self):

        np.random.shuffle(self.image_paths)
