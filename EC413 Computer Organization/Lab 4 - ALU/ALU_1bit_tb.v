`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/09/2024 08:20:02 PM
// Design Name: 
// Module Name: ALU_1bit_tb
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


module ALU_1bit_tb();
    reg R2, R3;
    reg [2:0] select;
    reg c_in;
    wire R1, c_out;

    ALU_1bit uut (
        .R2(R2),
        .R3(R3),
        .c_in(c_in),
        .select(select),
        .R1(R1),
        .c_out(c_out)
    );
    
    // Instantiate the behavioral ALU for comparison
    ALU_behavioral #(1) behavioral_dut (
        .R2(R2),
        .R3(R3),
        .ALUOp(select),
        .R1(R1_behavioral)
    );
    
    // Assign Error_flag
    assign error_flag = (R1 != R1_behavioral);
    
    // Verification logic
    always@(*)
        begin
        if(error_flag)
            // Use $display here instead of $monitor
            // $monitor will display the message whenever there's a change of a, b, c_in
            // $display will only display once when it's been executed
            $display("Error occurs when R1_Nbit = %d but R1_Behavioral = %d\n", R1, R1_behavioral);
        end

    initial begin
        // Test MOV
        R2 = 1'b1; R3 = 1'b0; select = 3'b000; c_in = 0;#10;
        $display("MOV: R1 = %b", R1); // Expected: R1 = 1

        // Test NOT
        R2 = 1'b1; select = 3'b001; c_in = 0;#10;
        $display("NOT: R1 = %b", R1); // Expected: R1 = 0

        // Test ADD
        R2 = 1'b1; R3 = 1'b1; select = 3'b010; c_in = 0; #10;
        $display("ADD: R1 = %b, Carry = %b", R1, c_out); // Expected: R1 = 0, Carry = 1
       
        R2 = 1'b0; R3 = 1'b1; select = 3'b010; c_in = 0;#10;
        $display("ADD: R1 = %b, Carry = %b", R1, c_out); // Expected: R1 = 1, Carry = 0

        // Test SUB
        R2 = 1'b1; R3 = 1'b0; select = 3'b011; c_in = 0;#10;
        $display("SUB: R1 = %b, Carry = %b", R1, c_out); // Expected: R1 = 1, Carry = 0
       
        R2 = 1'b0; R3 = 1'b0; select = 3'b011; c_in = 0;#10;
        $display("SUB: R1 = %b, Carry = %b", R1, c_out); // Expected: R1 = 0, Carry = 0

        // Test OR
        R2 = 1'b1; R3 = 1'b0; select = 3'b100; #10;
        $display("OR: R1 = %b", R1); // Expected: R1 = 1
       
        R2 = 1'b0; R3 = 1'b0; select = 3'b100; #10;
        $display("OR: R1 = %b", R1); // Expected: R1 = 0

        // Test AND
        R2 = 1'b1; R3 = 1'b1; select = 3'b101; #10;
        $display("AND: R1 = %b", R1); // Expected: R1 = 1
       
        R2 = 1'b1; R3 = 1'b0; select = 3'b101; #10;
        $display("AND: R1 = %b", R1); // Expected: R1 = 0

        // Test LESS THAN
        R2 = 1'b0; R3 = 1'b1; select = 3'b110; #10;
        $display("LESS THAN: R2 < R3 = %b < %b", R2, R3); // Expected: R1 = 1
       
        R2 = 1'b0; R3 = 1'b0; select = 3'b110; #10;
        $display("LESS THAN: R2 < R3 = %b < %b", R2, R3); // Expected: R1 = 0


        $finish;
    end
endmodule