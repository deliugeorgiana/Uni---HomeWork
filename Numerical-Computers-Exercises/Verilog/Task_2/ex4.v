module nor_gate(out, A, B);

input A,B;
output out;

supply1 pwr;
supply0 gnd;

nmos(out, gnd, A);
nmos(out, gnd, B);

pmos(out, pwr, A);
pmos(out, out, B);

endmodule

module testbench();

reg A,B;
wire out;
nor n(out, A, B);

initial begin
A=0; B=0;

#10 A=0; B=0;
#10 A=0; B=1;
#10 A=1; B=0;
#10 A=1; B=1;
#10 $finish;

end

initial $monitor("%b, %b, %b", A, B, out);

endmodule
