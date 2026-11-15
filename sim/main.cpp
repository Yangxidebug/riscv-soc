#include <cstdio>
#include <cstdint>
#include "Vcounter.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char** argv) {
    Verilated::commandArgs(argc, argv);
    Verilated::traceEverOn(true);

    const char* vcd_path = (argc > 1) ? argv[1] : "counter.vcd";

    Vcounter* dut = new Vcounter;
    VerilatedVcdC* tfp = new VerilatedVcdC;
    dut->trace(tfp, 99);
    tfp->open(vcd_path);

    vluint64_t t = 0;
    int errors = 0;

    auto tick = [&]() {
        dut->clk = 0; dut->eval(); tfp->dump(t++);
        dut->clk = 1; dut->eval(); tfp->dump(t++);
    };

    dut->rst_n = 0; dut->en = 0;
    tick(); tick();
    dut->rst_n = 1; dut->en = 1;

    for (int i = 0; i < 100; i++) {
        tick();
        uint8_t expect = (uint8_t)(i + 1);
        if (dut->cnt != expect) {
            printf("FAIL @ cycle %d: cnt=%u expect=%u\n", i, dut->cnt, expect);
            if (++errors > 5) break;
        }
    }

    if (!errors) printf("PASS: 100 cycles, cnt=%u\n", dut->cnt);

    tfp->close();
    delete tfp;
    delete dut;
    return errors ? 1 : 0;
}