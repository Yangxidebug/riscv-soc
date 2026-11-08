# RISC-V SoC on Gowin GW5AT-60

在 FPGA 上自研 AXI 总线，集成 VexRiscv（RV32IMAC + Sv32 MMU + I/D Cache），
最终跑通 U-Boot 与 Linux。

## 硬件

- 板卡：Sipeed Tang Mega 60K Dock
- 器件：GW5AT-LV60PG484A（59904 LUT4，2124 Kbit BSRAM）
- 内存：512 MiB DDR3，最高 1100 Mbps
- 工具：Gowin EDA、WSL2 + Verilator、Git

## 目录

| 路径 | 内容 |
| --- | --- |
| `soc/` | 可综合的 RTL |
| `sim/` | Verilator 的 C++ 测试与构建脚本 |
| `tb/` | Verilog testbench |
| `docs/decisions/` | 决策记录 |
| `docs/bugs/` | Bug 日志 |
| `docs/weekly/` | 周记 |
| `docs/evidence/` | 波形、报告、截图 |

## 进度

- [ ] v0.1 准备期：环境 + Verilog 语法 + FIFO + AXI-Lite 从机
- [ ] v0.2 自研总线通：AXI 交叉矩阵 + DDR 读写
- [ ] v0.3 裸机跑通
- [ ] v0.4 U-Boot 进命令行
- [ ] v0.5 Linux 进 shell
- [ ] v1.0 交付

当前状态：准备期，环境搭建中。