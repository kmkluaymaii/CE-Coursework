`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/10/2024 05:19:22 PM
// Design Name: 
// Module Name: Nbit_Subtracter
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


module Nbit_Subtracter #(parameter n = 8)(

    input [n-1:0] a,
    input [n-1:0] b,
    input b_in,
    output b_out,
    output [n-1:0] diff
);
    wire [n:0] carry;
    assign carry[0] = b_in;
    assign b_out = carry[n];
    genvar i;
    generate
    for(i = 0; i < n; i = i+1) begin
        FS_str Subtracter1Bit (carry[i+1], diff[i], a[i], b[i], carry[i]);
    end
    endgenerate
endmodule