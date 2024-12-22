`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/09/2024 08:16:56 PM
// Design Name: 
// Module Name: OR_T_2
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



`define	D		0	// definition of the delay

// Delayed OR gate

module OR_T_2(out, in1, in2);

input in1, in2;
output out;

or		#`D		or1 (out, in1, in2);


endmodule