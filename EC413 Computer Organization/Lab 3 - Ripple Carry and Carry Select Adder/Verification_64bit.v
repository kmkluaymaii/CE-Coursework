`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/30/2024 03:00:57 PM
// Design Name: 
// Module Name: Verification_64bit
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


module Verification_64bit(
output c_out, 
output [63:0]sum, 
input [63:0]a, 
input [63:0]b, 
input c_in
);

	assign {c_out, sum} = a + b + c_in;

endmodule

