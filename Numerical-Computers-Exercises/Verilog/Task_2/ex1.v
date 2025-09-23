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


module full_adder(A,B,Ci,S,Co);

input A,B,Ci;
output S,Co;
xor(S,A,B);
xor(w1,S,Ci);
and(w2,A,B);
and(w3,S,Ci);
or(Co,w2,w3);

endmodule

module half_adder(A,B,S,Co);

input A,B;
output S,Co;
xor(S,A,B);
and(Co,A,B);

endmodule

module testbench();

reg A0,B0,Ci,A1,B1,A2,B2,A3,B3;
wire S0,S1,S2,S3,C1,C2,C3,C4;

half_adder FA0(A0,B0,S0,C1);
full_adder FA1(A1,B1,C1,S1,C2);
full_adder FA2(A2,B2,C2,S2,C3);
full_adder FA3(A3,B3,C3,S3,C4);

initial begin
    $dumpfile("ex1.vcd");
    $dumpvars(0,testbench);

    A0 = 0; B0 = 0; Ci = 0;
    A1 = 0; B1 = 0;
    A2 = 1; B2 = 0;
    A3 = 1; B3 = 0;
    
    $monitor("%b, %b, %b, %b, %b, %b, %b, %b, %b, %b, %b,%b, %b, %b, %b,%b, %b", A0, B0, Ci, S0, A1, B1, C1, S1,A2,B2, C2, S2,A3, B3, C3, S3, C4);
    #10 A0 = 1;
    #10 B0 = 1;
    #10 A1 = 1;
    #10 B1 = 1;
    #10 A2 = 1;
    #10 B2 = 1;
    #10 A3 = 1;
    #10 B3 = 1;
    #10 $finish;

    end
endmodule