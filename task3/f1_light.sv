module f1_light#(
    parameter WIDTH = 16
)(
  input  logic             clk,  
  input  logic             rst,    
  input  logic             en,     
  input  logic [WIDTH-1:0] N,  
  output logic [7:0]       data_out
);

logic tick_to_en;

f1_fsm fsm (
    .rst(rst), .en(tick_to_en), .clk(clk), .data_out(data_out)
);

clktick #(.WIDTH(WIDTH))  tick_input(
    .N(N), .en(en), .rst(rst), .clk(clk), .tick(tick_to_en)
);

endmodule
