`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/09/2024 07:35:34 PM
// Design Name: 
// Module Name: ALU_Nbit
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




module ALU_Nbit #(parameter WIDTH = 32)(
  input signed [WIDTH-1:0] R2,
  input signed [WIDTH-1:0] R3,
  input c_in,
  input [2:0] ALUOp,
  input clk,
  output [WIDTH-1:0] R0,
  output c_out
);

wire [WIDTH-1:0] R1_reg;
wire [WIDTH:0] carry;
assign carry[0] = c_in;
assign c_out = carry[WIDTH];

assign R2_sign = R2[WIDTH-1];
assign R3_sign = R3[WIDTH-1];

reg [WIDTH-1:0] R1_temp;

genvar i;
generate
for (i = 0; i < WIDTH; i = i+1) begin
    ALU_1bit ALU1(R2[i], R3[i], carry[i], ALUOp, R1_reg[i], carry[i+1]);
end
endgenerate

always @(*) begin
    R1_temp <= R1_reg;
    if (ALUOp == 3'b110) begin
        if (R2_sign == 0 && R3_sign == 1) begin
            R1_temp <= 0;
        end else if (R2_sign == 1 && R3_sign == 0) begin
            R1_temp <= 1;
        end
    end

end

Nbit_Register #(WIDTH) Nbit_Register_1(R1_temp, R0, clk);

endmodule