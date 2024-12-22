`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/30/2024 02:02:54 PM
// Design Name: 
// Module Name: Adder_4bit
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


module Adder_4bit(
output c_out, 
output [3:0]sum, 
input [3:0]a, 
input [3:0]b, 
input c_in
);

wire FA1_out, FA2_out, FA3_out;

FA_str FA1 (FA1_out, sum[0], a[0], b[0], c_in);
FA_str FA2 (FA2_out, sum[1], a[1], b[1], FA1_out);
FA_str FA3 (FA3_out, sum[2], a[2], b[2], FA2_out);
FA_str FA4 (c_out, sum[3], a[3], b[3], FA3_out);

endmodule

