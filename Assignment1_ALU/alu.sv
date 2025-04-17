module alu(
    input logic [31:0] a, b,
    input logic [1:0] ALUControl,
    output logic [31:0] Result,
    output logic [3:0] ALUFlags // [3]=N, [2]=Z, [1]=C, [0]=V
);

    logic [31:0] B_in;
    logic [31:0] Sum;
    logic Carry_out;

    logic Sub;
    assign Sub = (ALUControl[1] == 1) && (ALUControl[0] == 1);

    assign B_in = Sub ? ~b : b;

    assign {Carry_out, Sum} = a + B_in + Sub;

    always_comb begin
        case (ALUControl)
            2'b00: Result = a & b;
            2'b01: Result = a | b;
            2'b10: Result = Sum;    //ADD
            2'b11: Result = Sum;    //SUB
            default: Result = 32'd0;
        endcase
    end

    // ALU_Flags [3]=N, [2]=Z, [1]=C, [0]=V
    assign ALUFlags[3] = Result[31];
    assign ALUFlags[2] = (Result == 32'd0);
    assign ALUFlags[1] = Carry_out;
    assign ALUFlags[0] = (~(a[31] ^ B_in[31]) & (a[31] ^ Sum[31]));

endmodule