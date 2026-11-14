import csv
from pathlib import Path
import matplotlib.pyplot as plt

BASE = Path(__file__).resolve().parent
DATA = BASE / "data" / "simd_avx_historical.csv"
FIGURES = BASE / "figures"
FIGURES.mkdir(exist_ok=True)

methods = []
times = []
speedups = []

with open(DATA, encoding="utf-8-sig", newline="") as f:
    for row in csv.DictReader(f):
        methods.append(row["method"])
        times.append(float(row["time_seconds"]))
        speedups.append(float(row["speedup"]))

plt.figure(figsize=(8, 5))
plt.bar(methods, times)
plt.ylabel("Execution Time (s)")
plt.title("AVX Optimization - Execution Time")
plt.tight_layout()
plt.savefig(FIGURES / "simd_avx_time.png", dpi=300)
plt.close()

plt.figure(figsize=(8, 5))
plt.bar(methods, speedups)
plt.ylabel("Speedup")
plt.title("AVX Optimization - Speedup")
plt.tight_layout()
plt.savefig(FIGURES / "simd_avx_speedup.png", dpi=300)
plt.close()

print("SIMD AVX figures generated successfully.")
