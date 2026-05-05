import tensorflow as tf

keras_model_path = "../smart_greenhouse/AI/models/1st_version/AI_model_v1.keras"
tflite_model_path = "../smart_greenhouse/AI/models/1st_version/AI_model_v1.tflite"

# Load model
model = tf.keras.models.load_model(keras_model_path)

# Create converter
converter = tf.lite.TFLiteConverter.from_keras_model(model)

# 🔥 Optimization (important for Raspberry Pi)
converter.optimizations = [tf.lite.Optimize.DEFAULT]

# 🔥 Ensure compatibility (prevents conversion errors)
converter.target_spec.supported_ops = [
    tf.lite.OpsSet.TFLITE_BUILTINS,
    tf.lite.OpsSet.SELECT_TF_OPS
]

# Convert
tflite_model = converter.convert()

# Save
with open(tflite_model_path, "wb") as f:
    f.write(tflite_model)

print("✅ TFLite model ready for Raspberry Pi")