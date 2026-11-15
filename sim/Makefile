# 一次仿真的构建脚本
#
# 用法（在本目录执行）：
#   make         编译并运行测试
#   make wave    编译、运行，然后用 GTKWave 打开波形
#   make clean   删掉所有构建产物
#
# 换模块时只改下面几行。注意注释要单独一行写，
# 写在变量后面会被当成变量值的一部分。

# 仓库根目录，由本文件位置推算，因此从本目录执行 make 即可
ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))/..)

TOP   := counter
RTL   := $(ROOT)/soc/counter.v
TB    := $(ROOT)/sim/main.cpp
VCD   := $(TOP).vcd

# 中间产物放 WSL 家目录，比放在 /mnt/f 快很多
BUILD := $(HOME)/vbuild/$(TOP)

# 故意不加 -Wno-fatal：警告即错误，早期宁可被打断
FLAGS := --cc --exe --build --trace -Wall

.PHONY: all wave clean

all:
	mkdir -p $(BUILD)
	cd $(ROOT) && verilator $(FLAGS) --top-module $(TOP) --Mdir $(BUILD) $(RTL) $(TB) -o V$(TOP)
	cd $(ROOT) && $(BUILD)/V$(TOP) $(VCD)

wave: all
	cd $(ROOT) && gtkwave $(VCD)

clean:
	rm -rf $(BUILD)
	cd $(ROOT) && rm -rf obj_dir *.vcd