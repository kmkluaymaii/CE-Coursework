`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/07/2024 01:58:48 PM
// Design Name: 
// Module Name: Nbit_Adder_tb
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


module Nbit_Adder_tb();
parameter n = 4;
reg [n-1:0] in1;
reg [n-1:0] in2;
reg c_in;

wire [n-1:0] out;
wire c_out;

Nbit_Adder #(n) adderTest(
    .a(in1),
    .b(in2),
    .sum(out),
    .c_in(c_in),
    .c_out(c_out)
);

initial begin

    in1 = 0;
    in2 = 0;
    c_in = 0;
    
    #100;
    
end

always
#2
{c_in, in1, in2} = {c_in, in1, in2} + 1;

endmodule


