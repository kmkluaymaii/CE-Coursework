`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 09/30/2024 03:01:57 PM
// Design Name: 
// Module Name: RCA_64bit_tb
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


module RCA_64bit_tb();

    // Inputs
	reg [63:0] a;
	reg [63:0] b;
	reg c_in;
	
	reg clk;

	// Outputs
	wire c_out;
	wire [63:0] sum;
	wire c_out_verify;
	wire [63:0] sum_verify;
	wire error_flag;

    
    // Instantiate the Unit Under Test (UUT)
	RCA_64bit A (
		.c_out(c_out), 
		.sum(sum), 
		.a(a), 
		.b(b), 
		.c_in(c_in)
	);
	
	// Verification module
	Verification_64bit Verification (
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
		
		a = 64'h0000000000000001;
        b = 64'h0000000000000001;
        c_in = 1'b0;

		#100;
		a = 64'hFFFFFFFFFFFFFFFF;
        b = 64'h0000000000000001;
        c_in = 1'b0;
        
        #100;
        a = 64'h0123456789ABCDEF;
        b = 64'hFEDCBA9876543210;
        c_in = 1'b0;

        #100;
        a = 64'h123456789ABCDEF0;
        b = 64'hEDCBA9876543210F;
        c_in = 1'b1;

        #100;
        a = 64'h0000000000000001;
        b = 64'h0000000000000002;
        c_in = 1'b0;

        #100;
        a = 64'h0000000000000003;
        b = 64'h0000000000000004;
        c_in = 1'b1;
        
        #100;
        
         $finish;

	end
      
endmodule
