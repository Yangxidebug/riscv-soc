// ================================================================
// det101 的 C++ 测试平台（testbench）
//
// 这个文件做三件事：
//   1. 例化 Verilator 生成的 C++ 模型 Vdet101（它是 soc/det101.v 翻译出来的）
//   2. 灌 6 拍输入，逐拍检查输出 z 是否符合预期
//   3. 把整个过程录成 det101.vcd，供 GTKWave 查看
//
// 在 sim/ 目录下运行：
//   make TOP=det101         编译并运行，看 PASS / FAIL
//   make TOP=det101 wave    编译、运行，再打开 GTKWave
//
// 正常输出：PASS: 6 cycles
// ================================================================

#include <cstdio>              // printf
#include <cstdint>             // uint8_t
#include "Vdet101.h"           // 【生成物】Verilator 从 det101.v 翻译出的类声明
#include "verilated.h"         // Verilator 运行库：eval()、仿真时间、参数解析
#include "verilated_vcd_c.h"   // VCD 波形记录器

int main(int argc, char** argv) {

    // ---- 初始化 ----------------------------------------------------
    // 让 Verilator 消化它自己的内置参数（以 +verilator+ 开头的那些），
    // 不调用的话遇到这类参数会报错。
    Verilated::commandArgs(argc, argv);

    // 波形记录的「总开关」。不打开这一句，后面的 trace() 什么都不会记。
    Verilated::traceEverOn(true);

    // 波形文件名从命令行第一个参数取；没给就用默认名。
    // Makefile 里传进来的是 det101.vcd。
    const char* vcd_path = (argc > 1) ? argv[1] : "det101.vcd";

    // 例化的是 C++ 类，不是 Verilog 模块。
    // 这个类的每个公开成员对应 det101.v 里的一个端口。
    Vdet101* dut = new Vdet101;

    // 波形记录器：负责把信号值写进 .vcd 文件
    VerilatedVcdC* tfp = new VerilatedVcdC;

    // 把模型和记录器连起来。第二个参数 99 表示记录 99 层层次结构，
    // 写大一点就行，目的是把 state 这种模块内部信号也录进去。
    dut->trace(tfp, 99);
    tfp->open(vcd_path);

    // ---- 时钟节拍函数 ----------------------------------------------
    // t 是仿真时间戳。每 dump 一次加一，单位是皮秒（ps）。
    vluint64_t t = 0;

    // tick() = 走完一个完整时钟周期：先低电平，再高电平。
    // 注意 eval() 是「重新计算一次」，它自己不会推进时间；
    // 时间是我们用 t 手动记的，仿真节奏完全由这个函数决定。
    auto tick = [&]() {
        // 低电平：更新组合逻辑，并记录一次波形
        dut->clk = 0;
        dut->eval();          // 重算：组合逻辑立即生效
        tfp->dump(t++);       // 记录当前所有信号值 → 波形上出现半个周期

        // 高电平：上升沿，时序逻辑（always @posedge clk）在这一步更新
        dut->clk = 1;
        dut->eval();
        tfp->dump(t++);       // 再记一次 → 波形上补全另外半个周期
    };

    // ---- 复位 ------------------------------------------------------
    // rst_n 是低有效异步复位。拉低后跑两拍，
    // 保证 always @(posedge clk or negedge rst_n) 真的把 state 清成 S0。
    dut->rst_n = 0;
    dut->x     = 0;
    tick();
    tick();
    dut->rst_n = 1;           // 释放复位，从下一拍开始正式测试

    // 以上两拍只是复位，不计入下面 6 拍的对照表。

    // ---- 激励与期望 ------------------------------------------------
    // xs[i]：第 i+1 拍要给检测器的输入
    // zs[i]：第 i+1 拍期望看到的输出
    // 这两行来自 note/02_FSM.md 里那张实测逐拍表。
    const uint8_t xs[6] = {1, 0, 1, 0, 1, 0};
    const uint8_t zs[6] = {0, 0, 0, 1, 0, 1};

    // ---- 主循环：逐拍比对 ------------------------------------------
    int errors = 0;

    for (int i = 0; i < 6; i++) {

        // ① 给这一拍设置输入
        dut->x = xs[i];

        // ② 让模型重新算一次，确保 z 已经反映最新状态。
        //    Moore 机的 z 只跟状态有关，x 变了 z 不会变，
        //    但保持这个习惯，换到 Mealy 或组合逻辑时就不会漏。
        dut->eval();

        // ③ 读「这一拍」的输出并比对
        //    关键：必须在 tick() 之前读，读到的才是这一拍的值
        if (dut->z != zs[i]) {
            printf("FAIL 拍%d: x=%u expect_z=%u actual_z=%u\n",
                   i + 1,
                   (unsigned)xs[i],
                   (unsigned)zs[i],
                   (unsigned)dut->z);
            if (++errors > 3) break;   // 错太多就停，避免刷屏
        }

        // ④ 时钟沿采样，进入下一拍
        //    这一步之后，state 才变成新的，z 也随之改变
        tick();
    }

    // ---- 结果汇总 --------------------------------------------------
    if (!errors) printf("PASS: 6 cycles\n");

    // 必须 close，否则 .vcd 文件不完整，GTKWave 可能打不开。
    tfp->close();
    delete tfp;
    delete dut;

    // 返回非零让 make 判为失败，脚本或 CI 可以直接看退出码。
    return errors ? 1 : 0;
}