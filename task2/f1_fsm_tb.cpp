#include "Vf1_fsm.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

int main(int argc, char **argv, char **env){
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);

    Vf1_fsm* top = new Vf1_fsm;

    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("f1_fsm.vcd");

    if(vbdOpen()!=1) return(-1);
    vbdHeader("Lab 3: F1 FSM");
    vbdSetMode(1); 

    top->clk = 1;
    top->rst = 1;
    top->en = 0;

    for(i=0; i<1000;i++){
        for(clk=0; clk<2; clk++){
            tfp->dump(2*i+clk);
            top->clk = !top->clk;
            top->eval ();
        }

        vbdCycle(i + 1);
        
        top->rst = (i<2) | (i==15) | (i==100);
        top->en = vbdFlag();
        vbdBar(top->data_out & 0xFF);
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();
    tfp->close();
    exit(0);
}
