`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/22/2024 06:01:29 PM
// Design Name: 
// Module Name: S2_Register
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


module S2_Register(
    input clk,
    input rst,
    input [31:0] Reg_ReadData1,
    input [31:0] Reg_ReadData2,
    input [15:0] S1_Imm,
	input S1_DataSrc,
	input [2:0]S1_ALUOp,
    input [4:0] S1_WriteSelect,
    input S1_WriteEnable,
    output reg [31:0] S2_ReadData1,
    output reg [31:0] S2_ReadData2,
    output reg [15:0] S2_Imm,
	output reg S2_DataSrc,
	output reg [2:0]S2_ALUOp,
    output reg [4:0] S2_WriteSelect,
    output reg S2_WriteEnable
   );

    always@(posedge clk)
        begin
        if (rst)
            begin
            S2_ReadData1   <= 32'd0;
            S2_ReadData2   <= 32'd0;
            S2_Imm <= 16'd0;
			S2_DataSrc <= 1'b0;
			S2_ALUOp <= 3'd0;
            S2_WriteSelect <= 5'd0;
            S2_WriteEnable <= 1'b0;
            end
        else
            begin
            S2_ReadData1   <= Reg_ReadData1;
            S2_ReadData2   <= Reg_ReadData2;
            S2_Imm <= S1_Imm;
			S2_DataSrc <= S1_DataSrc;
			S2_ALUOp <= S1_ALUOp;
            S2_WriteSelect <= S1_WriteSelect;
            S2_WriteEnable <= S1_WriteEnable;
            end
        end

endmodule

