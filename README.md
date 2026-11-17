# RISC-V SoC on Gowin GW5AT-60

在 FPGA 上自研 AXI 总线，集成 VexRiscv（RV32IMAC + Sv32 MMU + I/D Cache），
最终跑通 U-Boot 与 Linux。

## 硬件

- 板卡：小梅哥 ACG720-GW5AT60 教学开发板
- 器件：GW5AT-60，PBG484A（59904 LUT4，2124 Kbit BSRAM）
- 内存：256 MB DDR3
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

## 快速开始

需要 WSL2 + Ubuntu、Verilator 5.x、GTKWave。代码仓库在 Windows 盘，WSL 内通过 `/mnt/<盘符>` 访问。

```bash
cd /mnt/f/git/riscv-soc/sim
make         # 编译并运行，看到 PASS 即通
make wave    # 编译、运行，并用 GTKWave 打开波形
make clean   # 清理构建产物与波形
```

构建中间产物放在 `~/vbuild/<模块名>`，不落在仓库内。

## 当前状态

W1 完成（2026-11-15）：环境打通，Verilator 链路跑通，Verilog 语法至 FSM 已刷完。板卡未到货。