import os
from model_builder import build_model
from dataset_loader import PlantDataset
import numpy as np
from tensorflow.keras.callbacks import EarlyStopping,ModelCheckpoint # type: ignore

batch_size = 32
epochs = 20

train_dataset = PlantDataset(split="train", batch_size=batch_size)
val_dataset = PlantDataset(split="val", batch_size=batch_size)

class_num = len(train_dataset.class_names)
model= build_model(class_num)
early_stopping = EarlyStopping(
    monitor="val_loss",
    patience=3,
    restore_best_weights=True
)
checkpoint = ModelCheckpoint(
    "../smart_greenhouse/models/3rd_version/best_model_v3.keras",
    monitor="val_loss",
    save_best_only=True
)

model.fit(
    train_dataset,
    epochs= epochs,
    callbacks=[early_stopping, checkpoint], 
    validation_data=val_dataset,
)
os.makedirs("../smart_greenhouse/labels", exist_ok=True)
os.makedirs("../smart_greenhouse/models/3rd_version", exist_ok=True)

model.save("../smart_greenhouse/models/3rd_version/AI_model_v3.keras")
np.save("../smart_greenhouse/labels/class_names_v3.npy", train_dataset.class_names)