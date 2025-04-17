// alu_tb.sv
module alu_tb;
    logic [31:0] a, b;
    logic [1:0] ALUControl;
    logic [31:0] result;

    // DUT: Device Under Test
    alu uut (
        .a(a),
        .b(b),
        .ALUControl(ALUControl),
        .result(result)
    );

    initial begin
        $dumpfile("alu.vcd");
        $dumpvars(0, alu_tb);

        // test 1: ADD
        a = 10; b = 5; ALUControl = 2'b10; #10;
        $display("ADD: %d + %d = %d", a, b, result);

        // test 2: SUB
        a = 10; b = 5; ALUControl = 2'b11; #10;
        $display("SUB: %d - %d = %d", a, b, result);

        // test 3: AND
        a = 32'hA5A5A5A5; b = 32'hFFFF0000; ALUControl = 2'b00; #10;
        $display("AND: %h & %h = %h", a, b, result);

        // test 4: OR
        a = 32'h12345678; b = 32'h00FF00FF; ALUControl = 2'b01; #10;
        $display("OR : %h | %h = %h", a, b, result);

        $finish;
    end
endmodule
