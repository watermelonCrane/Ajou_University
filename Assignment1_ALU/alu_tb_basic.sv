`timescale 1ps/1ps

module alu_tb;

    logic [31:0] a, b;
    logic [1:0] ALUControl;
    logic [31:0] Result;
    logic [3:0] ALUFlags;

    alu uut (
        .a(a),
        .b(b),
        .ALUControl(ALUControl),
        .Result(Result),
        .ALUFlags(ALUFlags)
    );

    initial begin
        $dumpfile("alu_tb.vcd");
        $dumpvars(0, alu_tb);
    end

    initial begin
        $display("Test Start");
        a = 32'h00000005;
        b = 32'h00000003;

        //AND
        ALUControl = 2'b00;
        #10;
        $display("AND: Result=%h, Flags=%b", Result, ALUFlags);

        //OR
        ALUControl = 2'b01;
        #10;
        $display("ADD: Result=%h, Flags=%b", Result, ALUFlags);

        //ADD
        ALUControl = 2'b10;
        #10;
        $display("ADD: Result=%h, Flags=%b", Result, ALUFlags);

        //SUB
        ALUControl = 2'b11;
        #10;
        $display("SUB: Result=%h, Flags=%b", Result, ALUFlags);
        

        $display("etc");

        $display("Test End");
        $finish;
    end
endmodule
