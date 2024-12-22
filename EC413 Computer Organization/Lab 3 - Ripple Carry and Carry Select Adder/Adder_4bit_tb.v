`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/30/2024 02:11:18 PM
// Design Name: 
// Module Name: Adder_4bit_tb
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


module Adder_4bit_tb();

    // Inputs
	reg [3:0] a;
	reg [3:0] b;
	reg c_in;
	
	reg clk;

	// Outputs
	wire c_out;
	wire [3:0] sum;
	wire c_out_verify;
	wire [3:0] sum_verify;
	wire error_flag;

    
    // Instantiate the Unit Under Test (UUT)
	Adder_4bit A (
		.c_out(c_out), 
		.sum(sum), 
		.a(a), 
		.b(b), 
		.c_in(c_in)
	);
	
	// Verification module
	Verification_4bit Verification (
		.c_out(c_out_verify), 
		.sum(sum_verify), 
		.a(a), 
		.b(b), 
		.c_in(c_in)
	);
	
	// Assign Error_flag
	assign error_flag = (c_out != c_out_verify || sum != sum_verify);
	
	// Verification logic
	always@(posedge clk)
		begin
		if(error_flag)
			// Use $display here instead of $monitor
			// $monitor will display the message whenever there's a change of a, b, c_in
			// $display will only display once when it's been executed
			$display("Error occurs when a = %d, b = %d, c_in = %d\n", a, b, c_in);
		end
		
	// Derfine clk signal for Verfication purpose
	always #5 clk = ~clk;

	initial begin
		// Initialize Inputs
		clk = 0;
		
		a = 4'b0000;
		b = 4'b0000;
		c_in = 1;

		// Wait 10 ns
		#10;
		a = 4'b0001;
		b = 4'b0000;
		c_in = 1;

	end
	
	always #27  {a,b,c_in} ={a,b,c_in}+1;
      
endmodule

