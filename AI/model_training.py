import os
from model_builder import build_model
from dataset_loader import PlantDataset
import numpy as np

batch_size = 32


train_dataset = PlantDataset(split="train", batch_size=batch_size)
val_dataset = PlantDataset(split="val", batch_size=batch_size)


model= build_model()

train_features = []
for batch in train_dataset:
    features = model.predict(batch, verbose=0)
    train_features.append(features)
train_features = np.vstack(train_features)

mean_vector = np.mean(train_features, axis=0)
distances = np.linalg.norm(train_features - mean_vector, axis=1)
threshold = np.mean(distances) + 2 * np.std(distances)

os.makedirs("../smart_greenhouse/AI/models/1st_version", exist_ok=True)

model.save("../smart_greenhouse/AI/models/1st_version/AI_model_v1.keras")
np.save("../smart_greenhouse/AI/models/1st_version/mean_vector_v1.npy", mean_vector)
np.save("../smart_greenhouse/AI/models/1st_version/threshold_v1.npy", threshold)