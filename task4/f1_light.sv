module f1_light#(
    parameter N_WIDTH = 5, DELAY_WIDTH = 7
)(
  input  logic                 clk,
  input  logic                 rst,
  input  logic [N_WIDTH-1:0]   N,
  output logic [7:0]           data_out,
  output logic                 cmd_seq_o,
  output logic                 cmd_delay_o,
  input  logic                 trigger
);

logic tick_to_en;
logic time_out;
logic [DELAY_WIDTH-1:0] K;
logic mux_out;

f1_fsm fsm (
    .rst(rst), .en(mux_out), .clk(clk),
    .data_out(data_out), .trigger(trigger),
    .cmd_seq(cmd_seq_o), .cmd_delay(cmd_delay_o)
);

clktick #(.WIDTH(N_WIDTH))  tick_input(
    .N(N), .en(cmd_seq_o), .rst(rst), .clk(clk), .tick(tick_to_en)
);

lfsr_7 lfsr (
    .clk(clk), .rst(rst), .data_out(K[6:0])
);

delay #(.WIDTH(DELAY_WIDTH)) delay_trigger (
    .clk(clk), .rst(rst), .trigger(cmd_delay_o), .n(K), .time_out(time_out)
);

assign mux_out = cmd_seq_o ? tick_to_en : time_out;

endmodule
