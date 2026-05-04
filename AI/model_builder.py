from tensorflow.keras.applications import MobileNetV2 # type: ignore
from tensorflow.keras.layers import Dense, GlobalAveragePooling2D,Dropout # type: ignore
from tensorflow.keras.models import Model # type: ignore

#function to build the model using transfer learning with MobileNetV2 as the base model

def build_model():

    base_model = MobileNetV2(
        input_shape=(224,224,3),
        include_top=False,
        weights="imagenet"
    )
    base_model.trainable = False

    x = base_model.output
    x = GlobalAveragePooling2D()(x)
    x = Dense(128, activation="relu")(x)


    model = Model(inputs=base_model.input,outputs=x)


    return model