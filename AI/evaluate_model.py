import numpy as np
from tensorflow.keras.models import load_model # type: ignore
from dataset_loader import PlantDataset
import matplotlib.pyplot as plt # type: ignore

model=load_model("../smart_greenhouse/AI/models/1st_version/AI_model_v1.keras")
mean_vector=np.load("../smart_greenhouse/AI/models/1st_version/mean_vector_v1.npy")
threshold=np.load("../smart_greenhouse/AI/models/1st_version/threshold_v1.npy")

test_dataset = PlantDataset(split="test", batch_size=32)

y_true = []
y_pred = []
distances = []

for batch in test_dataset:
    features = model.predict(batch, verbose=0)
    for f in features:
        dist = np.linalg.norm(f - mean_vector)
        distances.append(dist)
        if dist < threshold:
            y_pred.append(0)  # Healthy
        else:
            y_pred.append(1)  # Unhealthy
        y_true.append(0)  # Assuming all test samples are healthy for this example

y_true=np.array(y_true)
y_pred=np.array(y_pred)
distances=np.array(distances)

accuracy = np.mean(y_true == y_pred)
print('\n===== Model Evaluation =====')
print('Accuracy:', accuracy)
print('Mean Distance:', np.mean(distances))
print('threshold:', threshold)

plt.hist(distances,bins=30)
plt.axvline(threshold, color='red', linestyle="--", label="Threshold")
plt.title("anomaly distance distribution")
plt.xlabel("distance from healthy center")
plt.ylabel("frequency")
plt.legend()
plt.show()