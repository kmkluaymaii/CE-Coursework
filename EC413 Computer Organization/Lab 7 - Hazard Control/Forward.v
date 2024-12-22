`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/04/2024 01:57:43 PM
// Design Name: 
// Module Name: Forward
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


module Forward(ForwardA, ForwardB, MemDest, WriteBackDest, IDEX_Rs, IDEX_Rt, EX_MEMRW, MEM_WBRW);
    output reg [1:0] ForwardA;
    output reg [1:0] ForwardB;
    input [4:0] MemDest; //EX/MEM.Rd
    input [4:0] WriteBackDest; //MEM/WB.Rd
    input [4:0] IDEX_Rs;
    input [4:0] IDEX_Rt;
    input EX_MEMRW;
    input MEM_WBRW;
    
    always @ (*)
     begin
        ForwardA = 2'b00;
        ForwardB = 2'b00;
        if (EX_MEMRW && (MemDest != 0) && (MemDest == IDEX_Rs)) begin
          ForwardA = 2'b10;
          end
        else if (MEM_WBRW && (WriteBackDest != 0) &&(WriteBackDest == IDEX_Rs)) begin
          ForwardA = 2'b01;
          end
          
        if(EX_MEMRW && (MemDest != 0) &&(MemDest == IDEX_Rt)) begin
          ForwardB = 2'b10;
          end
        else if (MEM_WBRW && (WriteBackDest != 0) &&(WriteBackDest == IDEX_Rt)) begin
          ForwardB = 2'b01;
          end
     end 
endmodule
