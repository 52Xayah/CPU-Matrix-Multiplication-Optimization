# Part09 · Packing 复用本地验证记录

> 状态更新（2026-11-15）：以下保留历史本地检查记录，不代表服务器性能验证。用户已要求停止源码优化；文末 Remaining server steps 仅保留为历史计划，不再作为当前执行任务。最新服务器观察见研究目录 Part10。

# G=2 Packing reuse verification — 2026-11-15

Independent source: src/packing_reuse/Matrix_mul_pack_reuse.c
Original source preserved: src/original/Matrix_mul.c
Original SHA256: 9298EDE786807EA0FD639371A17A42FF270EAB7B60DB807799B8CCFC3CC06228

Existing G=2 implementation was inspected, not recreated. MC=64, NC=128,
KC=256 and AVX 4x16 kernels remain. One private packed B panel serves two
adjacent row blocks; each block has its own accumulation buffer.
Added checked input parsing, finite seed validation, size overflow check,
and size_t indexing. stdout remains Trace only; elapsed_sec is stderr.
All existing directories, including empty directories, were preserved.

Actual local validation: Ubuntu WSL2, x86_64, GCC 13.3.0, AVX available.
Build flags: -std=gnu99 -O3 -mavx -fopenmp -ffp-contract=off -Wall -Wextra
162 exact Trace comparisons passed (18 dimensions, 3 seeds, 1/2/4 threads).
7 invalid-input checks passed. This is Trace validation, not full-matrix proof.
No benchmark data or speedup claim was produced.

Repeat from project root on Linux/WSL (Python 3 required):
    python3 tests/check_packing_reuse.py

Server access is not established. No local SSH config was found; no server
hostname/account was available. The local original copies match each other,
but have not been compared with the live server original.

Remaining server steps:
1. Confirm server original checksum and actual compiler/CPU environment.
2. Run the same correctness script on the target GCC/AVX/OpenMP environment.
3. Add a separately named timed baseline with identical timing boundaries;
   the original has no compute-only timer, so do not compare its process
   wall time directly with the optimized elapsed_sec.
4. Benchmark N=8192 with identical seeds, compiler options, thread affinity,
   warmup and repeated runs; retain raw outputs before computing medians.
5. Add full-matrix reference checks if correctness beyond Trace is required.
