`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/30/2024 01:57:11 PM
// Design Name: 
// Module Name: RCA_64bit
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


module RCA_64bit(
output c_out, 
output [63:0]sum, 
input [63:0]a, 
input [63:0]b, 
input c_in
);

wire RA1_out, RA2_out, RA3_out;

RCA_16bit RA1 (RA1_out, sum[15:0], a[15:0], b[15:0], c_in);
RCA_16bit RA2 (RA2_out, sum[31:16], a[31:16], b[31:16], RA1_out);
RCA_16bit RA3 (RA3_out, sum[47:32], a[47:32], b[47:32], RA2_out);
RCA_16bit RA4 (c_out, sum[63:48], a[63:48], b[63:48], RA3_out);

endmodule


