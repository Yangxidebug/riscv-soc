# 一次仿真的构建脚本
#
# 用法（在 sim/ 目录执行）：
#   make                   默认仿真 counter
#   make TOP=det101        仿真指定顶层模块
#   make wave TOP=det101   编译、运行，再用 GTKWave 打开波形
#   make clean TOP=det101  清掉该模块的构建目录与波形
#
# 命名约定（必须遵守，否则找不到文件）：
#   soc/<TOP>.v            设计源文件
#   sim/<TOP>_main.cpp     C++ 测试平台
#
# 换模块只需在命令行给 TOP，不必改本文件。
# 注意注释要单独一行写，写在变量后面会被当成变量值的一部分。

ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

TOP   ?= counter
RTL   ?= $(ROOT)/soc/$(TOP).v
TB    ?= $(ROOT)/sim/$(TOP)_main.cpp
VCD   ?= $(TOP).vcd

# 中间产物放 WSL 家目录，比放在 /mnt/f 快很多；按模块分目录，互不干扰
BUILD ?= $(HOME)/vbuild/$(TOP)

# 故意不加 -Wno-fatal：警告即错误，早期宁可被打断
FLAGS := --cc --exe --build --trace -Wall

.PHONY: all wave clean

all:
	@test -f $(RTL) || (echo "ERROR: 找不到 $(RTL)，检查 TOP 或文件命名"; exit 1)
	@test -f $(TB)  || (echo "ERROR: 找不到 $(TB)，检查 TOP 或文件命名";  exit 1)
	mkdir -p $(BUILD)
	cd $(ROOT) && verilator $(FLAGS) --top-module $(TOP) --Mdir $(BUILD) $(RTL) $(TB) -o V$(TOP)
	cd $(ROOT) && $(BUILD)/V$(TOP) $(VCD)

wave: all
	cd $(ROOT) && gtkwave $(VCD)

clean:
	rm -rf $(BUILD)
	cd $(ROOT) && rm -rf obj_dir $(VCD)