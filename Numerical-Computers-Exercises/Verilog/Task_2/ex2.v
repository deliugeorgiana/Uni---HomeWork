//inversor cmos, modul de test
module stimulus;
reg A;
wire Out;

//instantiere modul
my_not n1(Out,A);

//testare cazuri
initial begin
A=1'b0;
#10 A=1'b1;
#15 A=1'b0;
#20 A=1'b1;

end

initial
$monitor($time, " Out=%b  A=%b", Out, A);

endmodule

//modulul inversorului
module my_not(out, in);

input in;
output out;

//declarare surse de energie
supply1 pwr;
supply0 gnd;

//instantierea tranzistorilor nmos si pmos
nmos(out,gnd,in);
pmos(out,pwr,in);

endmodule