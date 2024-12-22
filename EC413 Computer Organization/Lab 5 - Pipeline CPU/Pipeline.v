`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/22/2024 06:02:53 PM
// Design Name: 
// Module Name: Pipeline
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


module Pipeline(
    input [31:0] InstrIn,
    input rst,
    input clk,
    input WriteEnable, 
    output [31:0] ALUOut
   );
   

    // The naming format for wire: Source_Destination_portname
    wire [4:0] S1_Reg_ReadSelect1;
    wire [4:0] S1_Reg_ReadSelect2;
    
    wire [15:0] S1_S2_Imm, S2_MUX_Imm;
    wire S1_S2_DataSrc, S2_MUX_DataSrc;
    wire [2:0] S1_S2_ALUOp, ALUOp;
    
    wire [4:0] S1_S2_WriteSelect, S3_Reg_WriteSelect;
    wire S1_S2_WriteEnable, S2_S3_WriteEnable, S3_Reg_WriteEnable;
    wire [31:0] Reg_S2_ReadData1, Reg_S2_ReadData2;
    wire [4:0] S2_Reg_WriteSelect, S2_S3_WriteSelect;
    wire [31:0] ALUResult;


    // Wires for ALU
    wire [31:0] R2, R3, R1, RD;

    S1_Register S1_Reg(
        .clk(clk),
        .rst(rst),
        .InstrIn(InstrIn),
        .WriteEnable(WriteEnable),
        .S1_ReadSelect1(S1_Reg_ReadSelect1),
        .S1_ReadSelect2(S1_Reg_ReadSelect2),
        
        .S1_Imm(S1_S2_Imm),
        .S1_DataSrc(S1_S2_DataSrc),
        .S1_ALUOp(S1_S2_ALUOp),
        
        .S1_WriteSelect(S1_S2_WriteSelect),
        .S1_WriteEnable(S1_S2_WriteEnable)
   );


    nbit_register_file Register_File (
        .WriteData(ALUResult),
        .ReadData1(Reg_S2_ReadData1),
        .ReadData2(Reg_S2_ReadData2),
        .ReadSelect1(S1_Reg_ReadSelect1),
        .ReadSelect2(S1_Reg_ReadSelect2),
        .WriteSelect(S3_Reg_WriteSelect),
        .WriteEnable(S3_Reg_WriteEnable),
        .Reset(rst),
        .Clk(clk)
    );


    S2_Register S2_Reg(
        .clk(clk),
        .rst(rst),
        .Reg_ReadData1(Reg_S2_ReadData1),
        .Reg_ReadData2(Reg_S2_ReadData2),
        .S1_Imm(S1_S2_Imm),
        .S1_DataSrc(S1_S2_DataSrc),
        .S1_ALUOp(S1_S2_ALUOp),
        
        .S1_WriteSelect(S1_S2_WriteSelect),
        .S1_WriteEnable(S1_S2_WriteEnable),
        
        .S2_ReadData1(R2),
        
        .S2_ReadData2(RD),
        .S2_Imm(S2_MUX_Imm),
        .S2_DataSrc(S2_MUX_DataSrc),
        
        .S2_ALUOp(ALUOp),
        
        .S2_WriteSelect(S2_S3_WriteSelect),
        .S2_WriteEnable(S2_S3_WriteEnable)
   );
   
   
   MUX MUX1(
        .MuxIn1(RD),
        .MuxIn2(S2_MUX_Imm),
        .MuxOut(R3),
        .MuxSel(S2_MUX_DataSrc)
   
   );
   
   Ideal_ALU ALU(
        .R1(R1),
        .R2(R2),
        .R3(R3),
        .ALUOp(ALUOp)
    );
   
    S3_Register S3_Reg(
        .clk(clk),
        .rst(rst),
        .ALUIn(R1),
        .S2_WriteSelect(S2_S3_WriteSelect),
        .S2_WriteEnable(S2_S3_WriteEnable),
        .ALUOut(ALUResult),
        .S3_WriteSelect(S3_Reg_WriteSelect),
        .S3_WriteEnable(S3_Reg_WriteEnable)
   );
   
   assign ALUOut = ALUResult;
   


endmodule