`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/30/2024 02:50:27 PM
// Design Name: 
// Module Name: RCA_16bit
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


module RCA_16bit(

output c_out, 
output [15:0]sum, 
input [15:0]a, 
input [15:0]b, 
input c_in
);

wire A1_out, A2_out, A3_out;

Adder_4bit A1 (A1_out, sum[3:0], a[3:0], b[3:0], c_in);
Adder_4bit A2 (A2_out, sum[7:4], a[7:4], b[7:4], A1_out);
Adder_4bit A3 (A3_out, sum[11:8], a[11:8], b[11:8], A2_out);
Adder_4bit A4 (c_out, sum[15:12], a[15:12], b[15:12], A3_out);

endmodule
