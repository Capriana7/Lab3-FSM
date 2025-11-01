module lfsr_7 (
    input   logic       clk,
    input   logic       rst,
    output  logic [6:0] data_out
);

    logic [7:1] sreg; 

    always_ff @(posedge clk)
        if (rst)
            sreg <= 7'b0001;
        else
            sreg <= {sreg[6:1], sreg[7] ^ sreg[3]};

    assign data_out = sreg;

endmodule
