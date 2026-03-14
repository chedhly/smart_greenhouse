import os
import cv2 as cv
import numpy as np
from tensorflow.keras.utils import Sequence # type: ignore
from image_treatment import ImageTreatment


class PlantDataset(Sequence):

    def __init__(self, dataset_path, batch_size=32):

        self.dataset_path = dataset_path
        self.batch_size = batch_size

        self.processor = ImageTreatment()

        self.image_paths = []
        self.labels = []

        classes = sorted(os.listdir(dataset_path))
        self.class_names = classes

        for idx, class_name in enumerate(classes):

            class_folder = os.path.join(dataset_path, class_name)

            for img in os.listdir(class_folder):

                self.image_paths.append(
                    os.path.join(class_folder, img)
                )

                self.labels.append(idx)

        self.labels = np.array(self.labels)


    def __len__(self):

        return len(self.image_paths) // self.batch_size


    def __getitem__(self, index):

        batch_paths = self.image_paths[
            index*self.batch_size:(index+1)*self.batch_size
        ]

        batch_labels = self.labels[
            index*self.batch_size:(index+1)*self.batch_size
        ]

        images = []

        for path in batch_paths:

            img = cv.imread(path)

            img = self.processor.preprocess(img)

            images.append(img)

        return np.array(images), batch_labels