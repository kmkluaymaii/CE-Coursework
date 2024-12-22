`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/30/2024 02:17:57 PM
// Design Name: 
// Module Name: Verification_4bit
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

module Verification_4bit(
output c_out, 
output [3:0]sum, 
input [3:0]a, 
input [3:0]b, 
input c_in
);

	assign {c_out, sum} = a + b + c_in;

endmodule

