module XOR(A,B,S);
    input A,B;
    output S;
    
    assign S = A ^ B;
endmodule

module AND(A,B,S);
    input A,B;
    output S;
    
    assign S = A & B;
endmodule

module OR(A,B,S);
    input A,B;
    output S;
    
    assign S = A | B;
endmodule


module st_fa(A, B, Ci, S, Co);
    input A, B, Ci;
    output wire S, Co; 
    
    wire w1, w2, w3;
    
    XOR x1(A, B, w1);
    XOR x2(w1, Ci, S);
    AND a1(w1, Ci, w2);
    AND a2(A, B, w3);
    OR o1(w2, w3, Co);
endmodule


module st_fa_tb();
    reg A, B, Ci;
    wire S, Co;
    
    st_fa uut(A, B, Ci, S, Co);

    initial begin
        $dumpfile("ex1.vcd"); 
        $dumpvars(0, st_fa_tb); 

        $monitor("Time=%0t A=%b B=%b Ci=%b -> S=%b Co=%b", $time, A, B, Ci, S, Co);
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

