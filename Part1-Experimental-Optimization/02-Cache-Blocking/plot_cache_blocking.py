import csv
from pathlib import Path
import matplotlib.pyplot as plt

BASE_DIR = Path(__file__).resolve().parent

DATA_FILE = BASE_DIR / "data" / "cache_blocking_historical.csv"
FIGURE_DIR = BASE_DIR / "figures"

FIGURE_DIR.mkdir(parents=True, exist_ok=True)

labels = []
times = []
speedups = []

with open(DATA_FILE, "r", newline="", encoding="utf-8") as f:
    reader = csv.DictReader(f)

    for row in reader:
        labels.append(row["block_label"])
        times.append(float(row["time_seconds"]))
        speedups.append(float(row["speedup"]))

# 图1：执行时间
plt.figure(figsize=(9, 5))
plt.bar(labels, times)
plt.xlabel("Block Size")
plt.ylabel("Execution Time (s)")
plt.title("Cache Blocking Performance (N=8192)")
plt.tight_layout()
plt.savefig(FIGURE_DIR / "cache_blocking_time.png", dpi=300)
plt.close()

# 图2：加速比
plt.figure(figsize=(9, 5))
plt.bar(labels, speedups)
plt.xlabel("Block Size")
plt.ylabel("Speedup")
plt.title("Cache Blocking Speedup (N=8192)")
plt.tight_layout()
plt.savefig(FIGURE_DIR / "cache_blocking_speedup.png", dpi=300)
plt.close()

print("Figures generated successfully.")