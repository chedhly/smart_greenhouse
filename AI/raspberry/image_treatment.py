import cv2 as cv
import numpy as np

class ImageTreatment:
    def __init__(self, 
                 target_size=(224,224),
                 blur_kernel=(5,5),
                 ):
        self.target_size = target_size
        self.blur_kernel = blur_kernel
        self.clahe = cv.createCLAHE(clipLimit=1.5, tileGridSize=(8,8))
        
        #determine if the image is too dark or too bright and apply CLAHE if necessary

    def apply_clahe(self, image):
        gray = cv.cvtColor(image, cv.COLOR_RGB2GRAY)
        min_val=np.percentile(gray, 5)
        max_val=np.percentile(gray, 95)
        spread=max_val-min_val
        if spread<80:
            lab=cv.cvtColor(image, cv.COLOR_RGB2LAB)
            l,a,b=cv.split(lab) 
            l=self.clahe.apply(l)
            lab=cv.merge((l,a,b))
            return cv.cvtColor(lab, cv.COLOR_LAB2RGB)
        return image

    def preprocess(self, image_bgr):
        # Convert BGR to RGB
        image_rgb=cv.cvtColor(image_bgr, cv.COLOR_BGR2RGB)
        # Resize to target size
        image_rgb=cv.resize(image_rgb, self.target_size, interpolation=cv.INTER_AREA)
        #clahe
        image_rgb=self.apply_clahe(image_rgb)
        # Normalize to [0,1]
        final_image=image_rgb.astype(np.float32) / 255.0
        return final_image
