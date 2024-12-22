`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/11/2024 01:17:24 AM
// Design Name: 
// Module Name: Nbit_Subtracter_tb
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


module Nbit_Subtracter_tb();
parameter n = 4;
reg [n-1:0] in1;
reg [n-1:0] in2;
reg b_in;

wire [n-1:0] diff;
wire b_out;

Nbit_Subtracter #(n) subtracterTest(
    .a(in1),
    .b(in2),
    .b_in(b_in),
    .b_out(b_out),
    .diff(diff)
);

initial begin

    in1 = 0;
    in2 = 0;
    b_in = 0;
    
    #100;
    
end

always
#2
{b_in, in1, in2} = {b_in, in1, in2} + 1;

endmodule


