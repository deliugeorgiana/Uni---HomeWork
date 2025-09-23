module ex3(out, a,b);
    input a,b;
    output out;
    
   wire c;

   supply1 pwr;
   supply0 gnd;

    pmos(out,pwr,a);
    pmos(out,pwr,b);

    nmos(out,c,a);
    nmos(c,gnd, b);

endmodule

module ex3_tb();
    reg a, b;
    wire out;
    
    ex3 uut(out, a, b);

    initial begin
        $dumpfile("ex3.vcd");  
        $dumpvars(0, ex3_tb);

        $monitor("Time=%0t a=%b b=%b -> out=%b", $time, a, b, out);

        a = 0; b = 0;
        #10 a = 0; b = 1;
        #10 a = 1; b = 0;
        #10 a = 1; b = 1;
        #10 $finish;  
    end
endmodule
