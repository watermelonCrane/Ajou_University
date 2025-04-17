`timescale 1ps/1ps

/*
* id = 202220757
* A = id; 202220757
* B = id - 2025; 202218732
*/


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

    //for timing diagram;
    initial begin
        $dumpfile("alu_tb.vcd");
        $dumpvars(0, alu_tb);
    end



    initial begin

        // Test (a)
        $display("Test Start (a)");
        a = 32'd202220757;
        b = 32'd202218732;

        ALUControl = 2'b11; //SUB
        #10;
        $display("SUB: Result=%h, Flags=%b", Result, ALUFlags);

        $display("Test End (a)");

        $display("============");



        // Test (b): NZCV
        $display("Test Start (b)");

        // Test N
        a = 32'd3;
        b = 32'd5;
        ALUControl = 2'b11;     // SUB
        #10
        $display("N: Result=%h, NZCV=%b", Result, ALUFlags);        // Result = fffffffe, NZCV = 1000


        // Test Z
        #20
        a = 32'd3;
        b = 32'd3;
        ALUControl = 2'b11;     // SUB
        #10
        $display("Z: Result=%h, NZCV=%b", Result, ALUFlags);        // Result = 00000000, NZCV = 0110


        // Test C
        #20
        a = 32'hFFFFFFFF;
        b = 32'h00000001;
        ALUControl = 2'b10;     // SUM
        #10
        $display("C: Result=%h, NZCV=%b", Result, ALUFlags);        // Result = 00000000, NZCV = 0110


        // Test V
        #20
        a = 32'h7FFFFFFF;
        b = 32'h00000001;
        ALUControl = 2'b10;     // SUM
        #10
        $display("V: Result=%h, NZCV=%b", Result, ALUFlags);       // Result = 80000000, NZCV = 1001
        $display("End (b)");

        $finish;

    end


endmodule
