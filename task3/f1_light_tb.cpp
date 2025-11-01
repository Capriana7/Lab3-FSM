#include "Vf1_light.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"
#define MAX_SIM_CYC 100000

int main(int argc, char **argv, char **env){
    int simcyc;     // simulation clock count
    int tick;       // each clk cycle has two ticks for two edges
    int lights = 0; // state to toggle LED lights

    Verilated::commandArgs(argc, argv);

    Vf1_light* top = new Vf1_light;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("f1_light.vcd");

    if(vbdOpen()!=1) return(-1);
    vbdHeader("Lab 3 Task 3: F1 FSM with Ticks");
    vbdSetMode(1); 

    top->clk = 1;
    top->rst = 0;
    top->en = 0;
    top->N = vbdValue();

    for(simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++){
        for (tick = 0; tick < 2; tick++){
            tfp->dump(2 * simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }
        vbdCycle(simcyc);
        top->rst = (simcyc < 2); // assert reset for 1st cycle
        top->en = (simcyc > 2);
        
        //top->en = vbdFlag();
        vbdBar(top->data_out & 0xFF);
        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
