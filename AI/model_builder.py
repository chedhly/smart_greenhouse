from tensorflow.keras.applications import MobileNetV2 # type: ignore
from tensorflow.keras.layers import Dense, GlobalAveragePooling2D # type: ignore
from tensorflow.keras.models import Model # type: ignore

#function to build the model using transfer learning with MobileNetV2 as the base model

def build_model(num_classes):

    base_model = MobileNetV2(
        input_shape=(224,224,3),
        include_top=False,
        weights="imagenet"
    )

    x = base_model.output

    x = GlobalAveragePooling2D()(x)

    x = Dense(128, activation="relu")(x)

    predictions = Dense(num_classes,
                        activation="softmax")(x)

    model = Model(inputs=base_model.input,
                  outputs=predictions)

    model.compile(
        optimizer="adam",
        loss="sparse_categorical_crossentropy",
        metrics=["accuracy"]
    )

    return model