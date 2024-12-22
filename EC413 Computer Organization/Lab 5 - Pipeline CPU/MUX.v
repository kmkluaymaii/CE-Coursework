`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/22/2024 07:06:19 PM
// Design Name: 
// Module Name: MUX
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


module MUX(MuxIn1, MuxIn2, MuxOut, MuxSel);

input [31:0]MuxIn1;
input [15:0]MuxIn2;
input MuxSel;
output [31:0]MuxOut;

assign MuxOut = MuxSel ? MuxIn2 : MuxIn1;
endmodule
