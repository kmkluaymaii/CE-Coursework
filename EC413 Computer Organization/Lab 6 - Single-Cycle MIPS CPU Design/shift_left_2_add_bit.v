`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/29/2024 08:05:34 PM
// Design Name: 
// Module Name: shift_left_2_add_bit
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


module shift_left_2_add_bit(in,out
    );
parameter size = 28;

input [size-3:0] in;
output [size-1:0] out;

assign out = {in,2'b00};

endmodule
