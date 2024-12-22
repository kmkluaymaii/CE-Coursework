`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/07/2024 01:49:55 PM
// Design Name: 
// Module Name: Nbit_Register_tb
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module Nbit_Register_tb();
parameter n = 16;
reg [n-1:0] d;
reg clk;

wire [n-1:0] q;

Nbit_Register #(n) registerTest(
    .out0(q),
    .in0(d),
    .clk(clk)
);

initial begin

    d = 0;
    clk = 0;
    
    #100;
    
end

always
#5 clk = ~clk;

always
#20 d = d+1;

endmodule
