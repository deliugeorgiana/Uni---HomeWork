module nor_gate(out, A, B);
    input A, B;
    output out;
    
    supply1 pwr;
    supply0 gnd;
    
    wire out_int;
    
    pmos(out_int, pwr, A);
    pmos(out, out_int, B);
    
    nmos(out, gnd, A);
    nmos(out, gnd, B);
endmodule


module mux_2to1(out, i0, i1, s);
    input i0, i1, s;
    output out;
    wire sbar;
    wire w0, w1;
  
    supply1 vdd;
    supply0 gnd;
    
    nmos(sbar, gnd, s);
    pmos(sbar, vdd, s);
    

    nand n0(w0, i0, sbar);
    nand n1(w1, i1, s);
    nand n2(out, w0, w1);
endmodule

module tb();
    reg i0, i1, s;
    wire out;

    mux_2to1 m1(out, i0, i1, s);

    initial begin
        $dumpfile("ex1.vcd");
        $dumpvars(0, tb);

        i0=0; i1=0; s=0;
        #10 i0=0; i1=1; s=0;
        #10 i0=1; i1=0; s=0;
        #10 i0=1; i1=1; s=0;
        #10 i0=0; i1=0; s=1;
        #10 i0=0; i1=1; s=1;
        #10 i0=1; i1=0; s=1;
        #10 i0=1; i1=1; s=1;
        #10 $finish;
    end

    initial $monitor("%b, %b, %b, %b", i0, i1, s, out);
endmodule