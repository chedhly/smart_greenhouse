# Azolla pinnata Disease / Stress Dataset — Organized

This folder contains images from `azolla/` copied into per-class sub-directories and
described by [`labels.csv`](labels.csv).

---

## Directory layout

```
azolla/organized/
├── README.md                  ← this file
├── labels.csv                 ← per-image metadata (all 58 images)
│
├── healthy/                   ← 18 images
├── heat_light_stress_red/     ← 25 images
├── blight_patch/              ←  9 images
├── nutrient_deficiency/       ←  2 images
├── algae_overgrowth/          ←  2 images
├── cyanobacteria_scum/        ←  1 image
└── chemical_injury/           ←  1 image
```

Originals in `azolla/` are **preserved unchanged**.  Images here are copies.

---

## Taxonomy

### Non-infectious (negative / reference classes)

| Class | Short description |
|---|---|
| `healthy` | Clear healthy *Azolla pinnata*: uniform bright green, intact fronds, no lesions or discoloration |
| `heat_light_stress_red` | Reddening / browning driven by high temperature or intense light (anthocyanin accumulation); frond structure still visible |
| `nutrient_deficiency` | Chlorosis (yellowing) or stunting consistent with N, P, or micronutrient shortage |
| `salinity_stress` | *(class defined, no images yet)* Wilting / bleaching / tip-burn caused by salt stress |
| `chemical_injury` | White or yellow bleaching; scorched tips or edges from herbicide / agrochemical exposure |
| `unknown_non_disease` | *(class defined, no images yet)* Stress suspected but abiotic cause unclear |

### Infectious / disease-like

| Class | Short description |
|---|---|
| `rot_soft` | *(class defined, no images yet)* Soft rot / frond collapse / damping-off; *Pythium*-like oomycete symptoms |
| `leaf_spot` | *(class defined, no images yet)* Discrete necrotic spots or lesions; *Alternaria*-/*Cercospora*-like |
| `blight_patch` | Larger blighted patches; generalised browning / mat collapse without clear frond softening |
| `sclerotial_rot` | *(class defined, no images yet)* White mycelium + sclerotia (*Sclerotium rolfsii*-like) |
| `unknown_disease` | *(class defined, no images yet)* Disease strongly suspected but pathogen / mechanism unclear |

### Biofouling / overgrowth

| Class | Short description |
|---|---|
| `algae_overgrowth` | Filamentous or mat-forming algae (*Spirogyra* spp., etc.) growing over or through the azolla mat |
| `cyanobacteria_scum` | Blue-green / pinkish scum or slime (cyanobacterial bloom) smothering the mat |

---

## Decision rules used for labelling

| Observed symptom | Assigned class | Notes |
|---|---|---|
| Uniform bright / medium green, intact fronds | `healthy` | High confidence if no visible stress markers |
| Orange, pink, red or reddish-brown fronds, structure intact | `heat_light_stress_red` | Azolla produces anthocyanin under high light / heat; very common |
| Yellow-green mat, washed-out colour, no reddening | `nutrient_deficiency` | Uniform chlorosis; N, P, Fe most likely |
| Green mat with white / pale bleached patches | `chemical_injury` | Herbicide / chemical damage produces bleaching rather than reddening |
| Dense uniform bright-green texture, very fine / filamentous | `algae_overgrowth` | Medium confidence — high-resolution confirmation recommended |
| Flat pinkish-red surface, no visible frond texture | `cyanobacteria_scum` | Medium confidence — lab/microscopy confirmation recommended |
| Widespread brown / black collapse, patchy; not purely red | `blight_patch` | Generalised blight; pathogen not yet isolated — medium confidence |
| Frond softening, watery collapse, localised | `rot_soft` | No confirmed examples in current dataset |
| Discrete small necrotic spots on fronds | `leaf_spot` | No confirmed examples in current dataset |

### Confidence levels

- **high** — Visual symptoms are unambiguous and consistent with the class definition.
- **medium** — Symptoms are consistent but overlapping classes are possible; lab / expert verification recommended before training.
- **low** — Highly ambiguous; use only in `unknown_*` classes.

---

## `labels.csv` columns

| Column | Description |
|---|---|
| `original_filename` | File name in `azolla/` (and in the class sub-folder) |
| `class` | Label class (see taxonomy above) |
| `confidence` | `high` / `medium` / `low` |
| `notes` | Short human rationale for the label |
| `source_url` | URL of original download (blank = unknown) |

---

## Dataset statistics (current)

| Class | Count |
|---|---|
| `healthy` | 18 |
| `heat_light_stress_red` | 25 |
| `blight_patch` | 9 |
| `nutrient_deficiency` | 2 |
| `algae_overgrowth` | 2 |
| `cyanobacteria_scum` | 1 |
| `chemical_injury` | 1 |
| **Total** | **58** |

Classes with 0 images (`rot_soft`, `leaf_spot`, `sclerotial_rot`, `salinity_stress`,
`unknown_disease`, `unknown_non_disease`) are defined in the taxonomy and ready to receive
images as the dataset grows.

---

## How to extend / relabel

### Adding new images

1. Place the raw image in `azolla/` (preserving originals).
2. Copy it into the appropriate `azolla/organized/<class>/` folder.
3. Append a row to `labels.csv` with all five fields filled in.

### Relabelling an existing image

1. Move the file copy from the current class folder to the new class folder:
   ```bash
   mv azolla/organized/old_class/imageX.jpg azolla/organized/new_class/
   ```
2. Update the `class`, `confidence`, and `notes` columns in `labels.csv`.

### Adding a new class

1. Create the folder: `mkdir azolla/organized/<new_class>`.
2. Add the class definition to this README (taxonomy table + decision rules).
3. Populate with images and update `labels.csv`.

### Splitting into train / val / test

Use a stratified split to maintain class proportions, e.g. with scikit-learn:

```python
from sklearn.model_selection import train_test_split
import pandas as pd

df = pd.read_csv('azolla/organized/labels.csv')
train, tmp = train_test_split(df, test_size=0.3, stratify=df['class'], random_state=42)
val, test  = train_test_split(tmp, test_size=0.5, stratify=tmp['class'], random_state=42)
```

---

## Notes on ambiguous images

Several images were labelled with **medium** confidence and may benefit from expert review:

- **`blight_patch`** images: the causative agent has not been identified; some images may
  actually represent severe `heat_light_stress_red` or `rot_soft` once examined more closely.
- **`algae_overgrowth`** (image50, image56): very fine/uniform green texture; could be a dense
  healthy azolla mat under some conditions — microscopy recommended.
- **`cyanobacteria_scum`** (image36): pinkish-red flat surface without frond texture; scum is
  the most likely explanation but should be confirmed.
- **`chemical_injury`** (image48): white bleached patches on otherwise green mat; abiotic
  bleaching is consistent but the specific cause is uncertain.
