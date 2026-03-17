import cv2 as cv
import numpy as np
import os
from image_treatment import ImageTreatment
from tensorflow.keras.models import load_model # type: ignore

model = load_model("../smart_greenhouse/models/AI_model.h5")
class_names = np.load("../smart_greenhouse/labels/class_names.npy", allow_pickle=True)

processer=ImageTreatment()

def predict_image(image_path):
    img=cv.imread(image_path)
    img=processer.preprocess(img)
    img=np.expand_dims(img, axis=0)

    predictions=model.predict(img)
    class_idx=np.argmax(predictions)
    class_name=class_names[class_idx]
    cofidence=predictions[0][class_idx]
    return class_name, cofidence

if __name__ == "__main__":
    image_path=input("Enter the path of the image to predict: ")
    if not os.path.exists(image_path):
        raise FileNotFoundError(f"Image not found: {image_path}")
    else:
        class_name, confidence = predict_image(image_path)
        print(f"Predicted class: {class_name}, Confidence: {confidence:.2f}")

