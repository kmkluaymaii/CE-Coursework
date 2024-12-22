`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/09/2024 06:32:39 PM
// Design Name: 
// Module Name: ALU_1bit
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


module ALU_1bit(
input R2,
input R3,
input c_in,
input [2:0] select,
output reg R1,
output reg c_out
    );
   
    wire mov_output;
    wire not_output;
    wire add_output;
    wire sub_output;
    wire or_output;
    wire and_output;
    wire less_output;
    wire c_out_add;
    wire b_out_sub;
    wire [2:0] mux_output;
    wire not_R3;
   
    // Move
    assign mov_output = R2;
   
    // NOT
    NOT NOT1(R2, not_output);
   
    // Addition
    Nbit_Adder #(1) Nbit_Adder_1(R2, R3, c_in, c_out_add, add_output);
   
    // Subtraction
    Nbit_Subtracter #(1) Nbit_Subtracter_1(R2, R3, c_in, b_out_sub, sub_output);
   
    // OR
    or or1(or_output,R2,R3);
   
    // AND
    and and1(and_output, R2, R3);
   

    //LESS THAN
    and and2(less_output, not_output, R3);

    always @(*) begin
      case(select)
        3'b000: begin
        R1 = mov_output;
        c_out = 0;
        end
        3'b001: begin
        R1 = not_output;
        c_out = 0;
        end
        3'b010: begin
        R1 = add_output;
        c_out = c_out_add;
        end
        3'b011: begin
        R1 = sub_output;
        c_out = b_out_sub;
        end
        3'b100: begin
        R1 = or_output;
        c_out = 0;
        end
        3'b101: begin
        R1 = and_output;
        c_out = 0;
        end
        3'b110: begin
        R1 = less_output;
        c_out = 0;
        end
      endcase
    end

   
endmodule