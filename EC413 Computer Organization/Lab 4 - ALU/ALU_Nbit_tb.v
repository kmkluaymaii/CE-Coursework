`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/10/2024 06:19:30 PM
// Design Name: 
// Module Name: ALU_Nbit_tb
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


module ALU_Nbit_tb #(parameter WIDTH = 32)();

    reg  signed [WIDTH-1:0] R2;
    reg  signed [WIDTH-1:0] R3;
    reg c_in;
    reg [2:0] ALUOp;
    reg clk;

    // Outputs
    wire [WIDTH-1:0] R1_Nbit;
    wire c_out;
    wire [WIDTH-1:0] R1_behavioral;

    // Instantiate the ALU_Nbit module
    ALU_Nbit #(WIDTH) dut (
        .R2(R2),
        .R3(R3),
        .c_in(c_in),
        .ALUOp(ALUOp),
        .clk(clk),
        .R0(R1_Nbit),
        .c_out(c_out)
    );

    // Instantiate the behavioral ALU for comparison
    ALU_behavioral #(WIDTH) behavioral_dut (
        .R2(R2),
        .R3(R3),
        .ALUOp(ALUOp),
        .R1(R1_behavioral)
    );
    
    // Assign Error_flag
    assign error_flag = (R1_Nbit != R1_behavioral);
    
    // Verification logic
    always@(posedge clk)
        begin
        if(error_flag)
            // Use $display here instead of $monitor
            // $monitor will display the message whenever there's a change of a, b, c_in
            // $display will only display once when it's been executed
            $display("Error occurs when R1_Nbit = %d but R1_Behavioral = %d\n", R1_Nbit, R1_behavioral);
        end
 

    // Clock generation
    initial begin
        clk = 0;
        forever #5 clk = ~clk; // 10 ns period
    end

    // Test procedure
    initial begin
        // Initialize inputs
        R2 = 0;
        R3 = 0;
        c_in = 0;
        ALUOp = 0;
       
        // Test MOV
        R2 = 32'hF0F0F0F0;
        ALUOp = 3'b000;
        #10;
        check_output();

        // Test NOT
        R2 = 32'h0F0F0F0F;
        ALUOp = 3'b001;
        #10;
        check_output();
       
        // Test ADD with normal values
        R2 = 32'h0000FFFF;
        R3 = 32'h00000001;
        ALUOp = 3'b010;
        #10;
        check_output();

        // Test ADD with overflow
        R2 = 32'h7FFFFFFF;
        R3 = 32'h00000001;
        ALUOp = 3'b010;
        #10;
        check_output();

        // Test ADD with negative values
        R2 = -32'h00000001;
        R3 = 32'h00000001;
        ALUOp = 3'b010;
        #10;
        check_output();

        // Test SUB with same values
        R2 = 32'h00000005;
        R3 = 32'h00000005;
        ALUOp = 3'b011;
        #10;
        check_output();

        // Test SUB resulting in negative
        R2 = 32'h00000005;
        R3 = 32'h00000006;
        ALUOp = 3'b011;
        #10;
        check_output();

        // Test SUB with zero
        R2 = 32'h00000000;
        R3 = 32'h00000005;
        ALUOp = 3'b011;
        #10;
        check_output();

        // Test OR
        R2 = 32'hAAAAAAAA;
        R3 = 32'h55555555;
        ALUOp = 3'b100;
        #10;
        check_output();

        // Test AND
        R2 = 32'hF0F0F0F0;
        R3 = 32'h0F0F0F0F;
        ALUOp = 3'b101;
        #10;
        check_output();

        // Test SLT
        R2 = 32'h00000002;
        R3 = 32'h00000003;
        ALUOp = 3'b110;
        #10;
        check_output();

        R2 = 32'hFFFFFFFF;
        R3 = 32'hFFFFFFFF;
        ALUOp = 3'b110;
        #10;
        check_output();

        R2 = 32'hFFFFFFFF;
        R3 = 32'h00000000;
        ALUOp = 3'b110;
        #10;
        check_output();

        R2 = 32'h00000000;
        R3 = 32'hFFFFFFFF;
        ALUOp = 3'b110;
        #10;
        check_output();

        // End simulation
        $finish;
    end

    // Monitor outputs
    initial begin
        $monitor("Time: %t | R2: %h | R3: %h | ALUOp: %b | R1_Nbit: %h | R1_behavioral: %h | c_out: %b",
                 $time, R2, R3, ALUOp, R1_Nbit, R1_behavioral, c_out);
    end

    // Task to check outputs
    task check_output;
        begin
            if (R1_Nbit !== R1_behavioral) begin
                $display("Mismatch at time %t: R1_Nbit = %h, R1_behavioral = %h", $time, R1_Nbit, R1_behavioral);
            end else begin
                $display("Match at time %t: R1_Nbit = %h", $time, R1_Nbit);
            end
        end
    endtask

endmodule

