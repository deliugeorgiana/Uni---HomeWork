module nand_gate(
    input wire A,  // Prima intrare
    input wire B,  // A doua intrare
    output wire Y  // Ieșirea NAND
);
    assign Y = ~(A & B);  // Realizează operația NAND
endmodule

`timescale 1ns/1ps

module nand_tb;
    reg A, B;      // Registre pentru intrări
    wire Y;        // Ieșirea NAND

    // Instanțierea modulului testat
    nand_gate uut (
        .A(A),
        .B(B),
        .Y(Y)
    );

    initial begin
        // Creează fișierul VCD
        $dumpfile("nand.vcd"); // Numele fișierului VCD
        $dumpvars(0, nand_tb); // Înregistrează toate variabilele din testbench

        // Afișează antetul pentru rezultate
        $monitor("Timp = %0t | A = %b | B = %b | Y = %b", $time, A, B, Y);
        
        A = 0; B = 0;
        #10; // Așteaptă 10 ns
        
        A = 0; B = 1;
        #10;

        A = 1; B = 0;
        #10;

        A = 1; B = 1;
        #10;
        
        // Finalizarea simulării
        $finish;
    end
endmodule
