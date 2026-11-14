# Part00 · 第二部分研究过程目录

本目录组织高性能矩阵乘法研究文档；第一部分已有章节编号，保持不变。

当前决定：停止源码优化，仅维护研究文档和真实实验记录。

| 顺序 | 文档 | 研究阶段 |
|---|---|---|
| Part01 | [实验环境与系统架构](../Part2-High-Performance-Implementation/docs/Part01_实验环境与系统架构.md) | 环境确认 |
| Part02 | [整体实现架构](../Part2-High-Performance-Implementation/docs/Part02_整体实现架构.md) | 执行流程 |
| Part03 | [多级矩阵分块设计](../Part2-High-Performance-Implementation/docs/Part03_多级矩阵分块设计.md) | 缓存与分块 |
| Part04 | [矩阵Packing技术分析](../Part2-High-Performance-Implementation/docs/Part04_矩阵Packing技术分析.md) | 数据布局 |
| Part05 | [AVX微内核与寄存器优化](../Part2-High-Performance-Implementation/docs/Part05_AVX微内核与寄存器优化.md) | 计算内核 |
| Part06 | [OpenMP与NUMA并行优化分析](../Part2-High-Performance-Implementation/docs/Part06_OpenMP与NUMA并行优化分析.md) | 线程与内存 |
| Part07 | [性能瓶颈分析与优化路线图](../Part2-High-Performance-Implementation/docs/Part07_性能瓶颈分析与优化路线图.md) | 待验证假设 |
| Part08 | [矩阵Packing复用优化方案](../Part2-High-Performance-Implementation/docs/Part08_矩阵Packing复用优化方案.md) | 历史方案，已暂停 |
| Part09 | [Packing复用本地验证记录](Part09_Packing复用本地验证记录.md) | 历史本地正确性检查 |
| Part10 | [服务器原版运行数据记录](../Part2-High-Performance-Implementation/docs/Part09_服务器原版运行数据记录.md) | 用户截图中的实测记录 |

两个 docs 目录均保留，使用链接串联，未移动文档到其他目录。
