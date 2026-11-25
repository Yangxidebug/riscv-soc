# RISC-V SoC on Gowin GW5AST-138

在 FPGA 上自研 AXI 总线，集成 VexRiscv（RV32IMAC + Sv32 MMU + I/D Cache），
最终跑通 U-Boot 与 Linux。

## 硬件

- 板卡：小梅哥 ACG720-138K 教学开发板
- 器件：GW5AST-138，PG484（Device Version C，138240 LUT4，6120 Kbit BSRAM）
- 内存：256 MB DDR3，16 位数据宽，1:4 时钟比，用户接口 128 位
- 工具：Gowin EDA V1.9.12.04、WSL2 + Verilator 5.x、Git

## 目录

| 路径 | 内容 |
| --- | --- |
| `soc/` | 可综合的 RTL |
| `sim/` | Verilator 的 C++ 测试与构建脚本 |
| `tb/` | Verilog testbench（暂未启用） |
| `docs/decisions/` | 决策记录 |
| `docs/bugs/` | Bug 日志 |
| `docs/weekly/` | 周记 |
| `docs/evidence/` | 波形、报告、截图 |

## 快速开始

需要 WSL2 + Ubuntu、Verilator 5.x、GTKWave。仓库在 Windows 盘，WSL 内通过 `/mnt/f` 访问。

```bash
cd /mnt/f/git/riscv-soc/sim

make TOP=det101          # 编译并运行指定模块，看到 PASS 即通
make wave TOP=det101     # 编译、运行，并用 GTKWave 打开波形
make clean TOP=det101    # 清理该模块的构建产物与波形
```

命名约定：`soc/<TOP>.v` 配 `sim/<TOP>_main.cpp`，两个名字对不上会直接报错。
构建中间产物放在 `~/vbuild/<TOP>`，不落在仓库内。

当前可跑的模块：`counter`、`det101`。

## 进度

- [ ] v0.1 准备期：环境 + Verilog 语法 + FIFO + AXI-Lite 从机
- [ ] v0.2 自研总线通：AXI 交叉矩阵 + DDR 读写
- [ ] v0.3 裸机跑通
- [ ] v0.4 U-Boot 进命令行
- [ ] v0.5 Linux 进 shell
- [ ] v1.0 交付

准备期细分进展：

| 项 | 状态 |
| --- | --- |
| 环境链路（Verilator + GTKWave + Gowin） | 完成 |
| Verilog 语法与状态机（HDLBits 至 FSM） | 完成 |
| 板卡选型与验收（ACG720-138K，DDR3 读写通过） | 完成 |
| AXI4 协议笔记 | 完成，参考代码分析待做 |
| 同步 / 异步 FIFO | 未开始 |
| AXI4-Lite 从机 | 未开始 |

## 当前状态

2026-11-25：准备期前段完成（环境、语法、板卡验收、AXI4 协议笔记），随后进入期末考试期暂停，
寒假从 AXI4-Lite 从机开工。

- 板卡验收与 DDR3 排查：[docs/bugs/0001-DDR3初始化失败与复位时序.md](docs/bugs/0001-DDR3初始化失败与复位时序.md)
- 关键取舍：[docs/decisions/0001-不使用AE350硬核.md](docs/decisions/0001-不使用AE350硬核.md)、[docs/decisions/0002-ddr3位宽与AXI数据宽.md](docs/decisions/0002-ddr3位宽与AXI数据宽.md)
