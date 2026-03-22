import numpy as np
from tensorflow.keras.models import load_model # type: ignore
from dataset_loader import PlantDataset
from sklearn.metrics import classification_report, confusion_matrix # type: ignore
import seaborn as sns # type: ignore
import matplotlib.pyplot as plt # type: ignore

model=load_model("../smart_greenhouse/models/3rd_version/AI_model_v3.keras")
class_names = np.load("../smart_greenhouse/labels/class_names_v3.npy", allow_pickle=True)

test_dataset = PlantDataset(split="test", batch_size=32)

y_true = []
y_pred = []

for batch_x, batch_y in test_dataset:
    predictions = model.predict(batch_x)
    predicted_classes = np.argmax(predictions, axis=1)
    y_true.extend(batch_y)
    y_pred.extend(predicted_classes)

y_true=np.array(y_true)
y_pred=np.array(y_pred)

print(classification_report(y_true, y_pred, target_names=class_names))

cm=confusion_matrix(y_true, y_pred)
plt.figure(figsize=(10,8))
sns.heatmap(cm, annot=True, fmt="d", xticklabels=class_names, yticklabels=class_names, cmap="Blues")
plt.xlabel("Predicted")
plt.ylabel("True")
plt.title("Confusion Matrix")
plt.show()