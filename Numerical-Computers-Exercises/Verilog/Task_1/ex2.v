module NAND(A,B,S);
    input A,B;
    output S;
    
    assign S = ~(A & B);
endmodule

module st_fa(A, B, Ci, Co, S);
    input wire A, B, Ci;
    output wire S, Co;

    wire w1, w2, w3,w4,w5,w6,w7,w8,w9;
    
    NAND n1(A, B, w1);
    NAND n2(w1, A, w2);
    NAND n3(w1, B, w3);
    NAND n4(w2, w3, w4); 

    NAND n5(w4, Ci, w5);
    NAND n6(w5, w4, w6);
    NAND n7(w5, Ci, w7);
    NAND n8(w6, w7, S);  

    
    NAND n9(A, B, w8);
    NAND n10(w4, Ci, w9);
    NAND n11(w8, w9, Co);  

endmodule

module st_fa_tb();
    reg A, B, Ci;
    wire S, Co;
    
    st_fa uut(A, B, Ci, S, Co);

    initial begin
        $dumpfile("ex2.vcd"); 
        $dumpvars(0, st_fa_tb); 

        $monitor("Time=%0t A=%b B=%b Ci=%b -> S=%b Cout=%b", $time, A, B, Ci, S, Co);
        A = 0; B = 0; Ci = 0;
        #10 A = 0; B = 0; Ci = 1;
        #10 A = 0; B = 1; Ci = 0;
        #10 A = 0; B = 1; Ci = 1;
        #10 A = 1; B = 0; Ci = 0;
        #10 A = 1; B = 0; Ci = 1;
        #10 A = 1; B = 1; Ci = 0;
        #10 A = 1; B = 1; Ci = 1;
        #10 $finish;
    end
endmodule