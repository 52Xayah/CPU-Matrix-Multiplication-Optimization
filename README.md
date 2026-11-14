# CPU Matrix Multiplication Optimization

CPU 矩阵乘法研究项目，包含 C 源码、缓存分块、AVX、OpenMP 分析、历史实验数据与图表。

## 阅读入口

- [研究过程目录](docs/Part00_研究过程目录.md)
- [服务器原版运行数据](Part2-High-Performance-Implementation/docs/Part09_服务器原版运行数据记录.md)
- 第一部分：`Part1-Experimental-Optimization/`
- 第二部分：`Part2-High-Performance-Implementation/`
- 原始源码：`src/original/Matrix_mul.c`

## 当前状态

保留原始源码，目前停止源码优化，仅整理研究文档和实际实验记录。
Packing 复用实验未取得服务器完成结果；本地验证文档仅为历史记录。
当前目录不含独立 Packing 复用源码，因此历史检查脚本不具备完整运行输入。

## 编译运行原版

在支持 AVX 的 Linux 环境、项目根目录下执行：

```bash
gcc -O3 -mavx -fopenmp -std=gnu99 -ffp-contract=off src/original/Matrix_mul.c -o matrix_original
OMP_NUM_THREADS=12 ./matrix_original 8192 0.3
```

以上为运行示例，不代表历史实验已确认采用相同编译选项或线程配置。
空目录通过 `.gitkeep` 保留。历史数据与本次服务器截图记录分别保留，不合并为同一次实验。
