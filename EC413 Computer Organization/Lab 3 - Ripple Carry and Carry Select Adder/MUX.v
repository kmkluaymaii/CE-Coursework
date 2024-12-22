`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/04/2024 02:37:39 PM
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


module MUX(
input [32:0] a,
input [32:0] b,
input select,
output [32:0] result,
input clk
);

wire [32:0] a, b;
reg [32:0] result;

always @(a,b,select)
begin
if(select==0)
    result=a;
else
    result=b;
end    

endmodule
