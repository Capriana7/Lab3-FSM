#include "Vf1_light.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <cstdio>

#define MAX_SIM_CYC 20000

int main(int argc, char **argv, char **env){
    Verilated::commandArgs(argc, argv);

    auto* top = new Vf1_light;

    Verilated::traceEverOn(true);
    auto* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("f1_light.vcd");

    top->clk = 1;
    top->rst = 1;
    top->trigger = 0;
    top->N = 24;

    int lights_prev = -1;
    int simcyc, tick;

    std::printf("[TB] start\n"); std::fflush(stdout);

    for (simcyc = 0; simcyc < MAX_SIM_CYC; ++simcyc) {
        top->rst = (simcyc < 2);
        top->N   = 24;

        // no Vbuddy: hold trigger low; uncomment next line to inject a press at cycle 1000
        // if (simcyc == 1000) top->trigger = 1; else if (simcyc == 1002) top->trigger = 0;

        for (tick = 0; tick < 2; ++tick) {
            top->clk = !top->clk;
            top->eval();
            tfp->dump(2*simcyc + tick);
        }

        int lights = top->data_out & 0xFF;
        if (lights != lights_prev) {
            std::printf("[TB] cyc=%d data_out=0x%02X\n", simcyc, lights);
            std::fflush(stdout);
            lights_prev = lights;
        }

        if (Verilated::gotFinish()) break;
    }

    std::printf("[TB] done\n"); std::fflush(stdout);
    tfp->close();
    delete top;
    return 0;
}
