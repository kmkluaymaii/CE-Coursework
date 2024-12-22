`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/04/2024 04:19:20 PM
// Design Name: 
// Module Name: RCA_32bit
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


module RCA_32bit(
output c_out, 
output [31:0]sum, 
input [31:0]a, 
input [31:0]b, 
input c_in
);

wire RA1_out;

RCA_16bit RA4 (RA1_out, sum[15:0], a[15:0], b[15:0], c_in);
RCA_16bit RA5 (c_out, sum[31:16], a[31:16], b[31:16], RA1_out);

endmodule
