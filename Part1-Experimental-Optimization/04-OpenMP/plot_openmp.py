import csv
from pathlib import Path
import matplotlib.pyplot as plt

BASE = Path(__file__).resolve().parent
DATA = BASE / "data" / "openmp_historical.csv"
FIGURES = BASE / "figures"
FIGURES.mkdir(parents=True, exist_ok=True)

threads, times, speedups, efficiencies = [], [], [], []

with open(DATA, encoding="utf-8-sig", newline="") as f:
    for row in csv.DictReader(f):
        threads.append(int(row["threads"]))
        times.append(float(row["time_seconds"]))
        speedups.append(float(row["speedup"]))
        efficiencies.append(float(row["efficiency_percent"]))

# 图1：执行时间
plt.figure(figsize=(9, 5))
plt.plot(threads, times, marker="o")
plt.xlabel("Number of Threads")
plt.ylabel("Execution Time (s)")
plt.title("OpenMP - Execution Time")
plt.xticks(threads)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig(FIGURES / "openmp_execution_time.png", dpi=300)
plt.close()

# 图2：加速比及理想线性加速
plt.figure(figsize=(9, 5))
plt.plot(threads, speedups, marker="o", label="Actual Speedup")
plt.plot(threads, threads, linestyle="--", label="Ideal Linear Speedup")
plt.xlabel("Number of Threads")
plt.ylabel("Speedup")
plt.title("OpenMP - Speedup")
plt.xticks(threads)
plt.legend()
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig(FIGURES / "openmp_speedup.png", dpi=300)
plt.close()

# 图3：并行效率
plt.figure(figsize=(9, 5))
plt.plot(threads, efficiencies, marker="o")
plt.xlabel("Number of Threads")
plt.ylabel("Parallel Efficiency (%)")
plt.title("OpenMP - Parallel Efficiency")
plt.xticks(threads)
plt.ylim(0, 110)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig(FIGURES / "openmp_efficiency.png", dpi=300)
plt.close()

print("OpenMP figures generated successfully!")
