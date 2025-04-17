// alu.sv
module alu (
    input logic [31:0] a,
    input logic [31:0] b,
    input logic [1:0] ALUControl,
    output logic [31:0] result
);
    always_comb begin
        case (ALUControl)
            2'b00: result = a & b;
            2'b01: result = a | b;
            2'b10: result = a + b;
            2'b11: result = a - b;
            default: result = 32'hDEADBEEF;
        endcase
    end
endmodule
