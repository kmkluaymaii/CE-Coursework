`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/10/2024 05:25:25 PM
// Design Name: 
// Module Name: FS_str
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



module FS_str(b_out, diff, a, b, b_in);

//parameter D = 0;

input a, b, b_in; //declare inputs a, b and b_in, 1 bit each
output b_out, diff; //declare outputs b_out and diff, 1 bit each
//declare wires for values being passed between different gates
wire not_a, not_b, not_bin;
wire and1_out, and2_out, and3_out, and4_out;
wire and5_out, and6_out, and7_out;

not   not1 (not_a, a);
not   not2 (not_b, b);
not not3 (not_bin, b_in);

// get sum
and and1 (and1_out, not_a, b, not_bin);
and and2 (and2_out, not_a, not_b, b_in);
and and3 (and3_out, a, not_b, not_bin);
and and4 (and4_out, a, b, b_in);
or or1 (diff, and1_out, and2_out, and3_out, and4_out);

// get c_out
and and5 (and5_out, not_a, b);
and and6 (and6_out, not_a, b_in);
and and7 (and7_out, b, b_in);
or or2 (b_out, and5_out, and6_out, and7_out);


endmodule
