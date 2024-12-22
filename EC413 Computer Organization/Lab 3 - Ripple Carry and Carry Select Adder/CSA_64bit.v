`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/30/2024 02:50:11 PM
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


module CSA_64bit(
output c_out, 
output [63:0]out, 
input [63:0]a, 
input [63:0]b, 
input c_in
);

wire select, RCA1_out, RCA2_out;
wire [31:0] sum1, sum2;
wire c_in0 = 0;
wire c_in1 = 1;

wire [32:0]c;
wire [32:0]d;

wire [32:0]mux_out;


RCA_32bit RCA0 (select, out[31:0], a[31:0], b[31:0], c_in);
RCA_32bit RCA1 (RCA1_out, sum1[31:0], a[63:32], b[63:32], c_in0);
RCA_32bit RCA2 (RCA2_out, sum2[31:0], a[63:32], b[63:32], c_in1);

assign c = {sum1, RCA1_out};
assign d = {sum2, RCA2_out};


MUX MUX1 (c[32:0],d[32:0],select,mux_out[32:0]);

assign c_out = mux_out[0];
assign out[63:32] = mux_out[32:1];

endmodule

