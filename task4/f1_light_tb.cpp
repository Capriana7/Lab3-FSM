#include "Vf1_light.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

#define MAX_SIM_CYC 100000

int main(int argc, char **argv, char **env){
    int simcyc, tick;
    int lights = 0, prev_lights = -1;
    int btn = 0;
    bool timing = false;
    int elapsed = 0;
    bool vb = false;
    int trigger_pulse = 0;

    Verilated::commandArgs(argc, argv);

    auto* top = new Vf1_light;

    Verilated::traceEverOn(true);
    auto* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("f1_light.vcd");

    if (vbdOpen()==1) vb = true;
    if (vb) { vbdHeader("Lab 3 Task 4: F1 Light"); vbdSetMode(1); }

    top->clk = 1;
    top->rst = 1;
    top->N = 24;
    top->trigger = 0;

    for (simcyc = 0; simcyc < MAX_SIM_CYC; ++simcyc) {
        top->rst = (simcyc < 2);
        top->N   = 24;

        btn = vb ? vbdFlag() : 0;
        trigger_pulse = (!timing && btn) ? 1 : 0;
        top->trigger = trigger_pulse;

        for (tick = 0; tick < 2; ++tick) {
            top->clk = !top->clk;
            top->eval();
            tfp->dump(2*simcyc + tick);
        }

        lights = top->data_out & 0xFF;
        if (vb) vbdBar(lights);

        if (!timing && prev_lights != -1 && prev_lights != 0x00 && lights == 0x00) {
            if (vb) { vbdInitWatch(); vbdHeader("ARMED"); }
            timing = true;
        }

        if (timing && btn) {
            if (vb) {
                elapsed = vbdElapsed();
                char buf[40];
                std::sprintf(buf, "RT: %d ms", elapsed);
                vbdHeader(buf);
                vbdHex(0, (elapsed >> 0)  & 0xF);
                vbdHex(1, (elapsed >> 4)  & 0xF);
                vbdHex(2, (elapsed >> 8)  & 0xF);
                vbdHex(3, (elapsed >> 12) & 0xF);
                vbdHex(4, (elapsed >> 16) & 0xF);
            }
            break;
        }

        prev_lights = lights;

        vbdCycle(simcyc);
        if ((Verilated::gotFinish()) || (vbdGetkey()=='q')) 
            exit(0);  
    }

    vbdClose();
    tfp->close();
    return 0;
}
