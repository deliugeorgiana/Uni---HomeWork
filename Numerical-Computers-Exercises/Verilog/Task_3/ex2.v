module bistabil(Q ,D,CK);

    input D, CK;
    output Q;
    
    supply1 VDD;
    supply0 GND;
    
    wire CKbar;      
    wire w0,w1,w2;
  
    cmos ck_inv(CKbar, CK, CK, CK);

    cmos p1(w0, D, CK, CKbar);    
    cmos p2(w2, w1, CKbar, CK);   
    

    cmos inv1(w1, w0, w0, w0);   
    cmos inv2(Q, w2, w2, w2); 
endmodule

module tb();
    reg D, CK;
    wire Q;

    bistabil b(Q, D, CK);

    initial begin
    $dumpfile("ex2.vcd");
    $dumpvars(0, tb);

        D=0; CK=0;
        #10 D = 0; CK = 1;
        #10 CK = 0;

        #10 D = 1; CK = 0;
        #10 CK = 1;
        #10 CK = 0;

        #10 $finish;
    end

    initial $monitor("%b, %b, %b", D, CK, Q);
endmodule

