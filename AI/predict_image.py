from platform import processor

import cv2 as cv
import numpy as np
import os
from image_treatment import ImageTreatment
from tensorflow.keras.models import load_model # type: ignore

model = load_model("../smart_greenhouse/AI/models/1st_version/AI_model_v1.keras")
mean_vector = np.load("../smart_greenhouse/AI/labels/mean_vector_v1.npy")
threshold = np.load("../smart_greenhouse/AI/labels/threshold_v1.npy")

def tank_fullness_prediction(image_path):
    img=cv.imread(image_path)
    img_hsv=cv.cvtColor(img, cv.COLOR_BGR2HSV)
    
    lower_green=np.array([36, 25, 25])
    upper_green=np.array([70, 255, 255])
    mask=cv.inRange(img_hsv, lower_green, upper_green)
    num_lable,lables,stats,_=cv.connectedComponentsWithStats(mask)
    min_area=100
    clean_mask=np.zeros_like(mask)
    for i in range(1, num_lable):
        area=stats[i, cv.CC_STAT_AREA]
        if area >= min_area:
            clean_mask[lables == i] = 255
    clean_mask=cv.morphologyEx(clean_mask, cv.MORPH_CLOSE, np.ones((3,3), np.uint8))        
    
    azolla_pixels=np.count_nonzero(clean_mask)
    total_pixels=clean_mask.size

    fullness_percentage=(azolla_pixels / total_pixels) * 100
    return fullness_percentage,clean_mask

def predict_image(image_path):
    img=cv.imread(image_path)
    img=processor.preprocess(img)
    img=np.expand_dims(img, axis=0)

    features=model.predict(img)[0]
    dist=np.linalg.norm(features - mean_vector)

    if dist < threshold:
        return "Healthy", dist
    else:
        return "Unhealthy", dist

if __name__ == "__main__":
    image_path=input("Enter the path of the image to predict: ")
    if not os.path.exists(image_path):
        raise FileNotFoundError(f"Image not found: {image_path}")
    else:
        class_name, confidence = predict_image(image_path)
        fullness_percentage, clean_mask = tank_fullness_prediction(image_path)
        print(f"Predicted class: {class_name}, Confidence: {confidence:.2f}")
        print(f"Tank Fullness: {fullness_percentage:.2f}%")
        cv.imshow("Clean Mask", clean_mask)
        cv.waitKey(0)
        cv.destroyAllWindows()
