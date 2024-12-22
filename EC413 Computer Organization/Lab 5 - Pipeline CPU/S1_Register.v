`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/22/2024 06:00:26 PM
// Design Name: 
// Module Name: S1_Register
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

module S1_Register(
	input clk,
	input rst,
	input [31:0] InstrIn,
	input WriteEnable,
	output reg [4:0] S1_ReadSelect1,
	output reg [4:0] S1_ReadSelect2,
	output reg [15:0] S1_Imm,
	output reg S1_DataSrc,
	output reg [2:0]S1_ALUOp,
	output reg [4:0] S1_WriteSelect,
	output reg S1_WriteEnable
   );

	always@(posedge clk)
		begin
		if (rst)
			begin
			S1_ReadSelect1 <= 5'd0;
			S1_ReadSelect2 <= 5'd0;
			S1_Imm <= 16'd0;
			S1_DataSrc <= 1'b0;
			S1_ALUOp <= 3'd0;
			S1_WriteSelect <= 5'd0;
			S1_WriteEnable <= 1'b0;
			end
		else
			begin
			S1_ReadSelect1 <= InstrIn[20:16];
			S1_ReadSelect2 <= InstrIn[15:11];
			S1_Imm <= InstrIn[15:0];
			S1_DataSrc <= InstrIn[29];
			S1_ALUOp <= InstrIn[28:26];
			S1_WriteSelect <= InstrIn[25:21];
			S1_WriteEnable <= WriteEnable;
			end
		end

endmodule
