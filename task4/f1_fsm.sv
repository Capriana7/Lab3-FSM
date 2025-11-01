module f1_fsm (
    input  logic       rst,
    input  logic       en,
    input  logic       clk,
    input  logic       trigger,
    output logic [7:0] data_out,
    output logic       cmd_seq,
    output logic       cmd_delay
);
    typedef enum logic [3:0] {S0,S1,S2,S3,S4,S5,S6,S7,S8} my_state;
    my_state current_state, next_state;

    logic trigger_q, trig_rise;
    logic seq_mode;

    always_ff @(posedge clk) begin
        trigger_q <= trigger;
    end
    assign trig_rise = trigger & ~trigger_q;

    always_ff @(posedge clk) begin
        if (rst) begin
            current_state <= S0;
            seq_mode      <= 1'b0;
            cmd_delay     <= 1'b0;
        end else begin
            current_state <= next_state;
            cmd_delay <= 1'b0;
            if (!seq_mode && (current_state==S0) && trig_rise)
                seq_mode <= 1'b1;
            if (seq_mode && (current_state==S8) && en) begin
                seq_mode  <= 1'b0;
                cmd_delay <= 1'b1;
            end
        end
    end

    always_comb begin
        next_state = current_state;
        if (en) begin
            unique case (current_state)
                S0: next_state = seq_mode ? S1 : S0;
                S1: next_state = S2;
                S2: next_state = S3;
                S3: next_state = S4;
                S4: next_state = S5;
                S5: next_state = S6;
                S6: next_state = S7;
                S7: next_state = S8;
                S8: next_state = seq_mode ? S8 : S0;
                default: next_state = S0;
            endcase
        end
    end

    assign cmd_seq  = seq_mode;
    assign data_out = (8'b1 << current_state) - 1;
endmodule
