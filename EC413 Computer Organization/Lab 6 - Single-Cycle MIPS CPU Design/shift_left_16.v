`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/29/2024 05:27:49 PM
// Design Name: 
// Module Name: shift_left_16
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


module shift_left_16(in,out
    );
parameter size = 32;

input [size-17:0] in;
output [size-1:0] out;

assign out = {in,16'b0000000000000000};

endmodule
