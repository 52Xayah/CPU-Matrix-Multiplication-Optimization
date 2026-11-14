import csv
from pathlib import Path
import matplotlib.pyplot as plt

BASE = Path(__file__).resolve().parent
DATA = BASE / "data" / "performance_historical.csv"
FIGURES = BASE / "figures"

FIGURES.mkdir(parents=True, exist_ok=True)

methods, times, speedups, gflops = [], [], [], []

with open(DATA, encoding="utf-8-sig", newline="") as f:
    for row in csv.DictReader(f):
        methods.append(row["method"])
        times.append(float(row["time_seconds"]))
        speedups.append(float(row["speedup"]))
        gflops.append(float(row["gflops"]))

# 图1：执行时间（对数坐标）
plt.figure(figsize=(10, 6))
plt.bar(methods, times)
plt.yscale("log")
plt.ylabel("Execution Time (s, log scale)")
plt.title("Matrix Multiplication - Execution Time")
plt.xticks(rotation=15, ha="right")
plt.tight_layout()
plt.savefig(FIGURES / "performance_time.png", dpi=300)
plt.close()

# 图2：加速比
plt.figure(figsize=(10, 6))
plt.bar(methods, speedups)
plt.ylabel("Speedup")
plt.title("Matrix Multiplication - Speedup")
plt.xticks(rotation=15, ha="right")
plt.tight_layout()
plt.savefig(FIGURES / "performance_speedup.png", dpi=300)
plt.close()

# 图3：GFLOPS
plt.figure(figsize=(10, 6))
plt.bar(methods, gflops)
plt.ylabel("GFLOPS")
plt.title("Matrix Multiplication - Computational Performance")
plt.xticks(rotation=15, ha="right")
plt.tight_layout()
plt.savefig(FIGURES / "performance_gflops.png", dpi=300)
plt.close()

print("Performance comparison figures generated successfully!")
