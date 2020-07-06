#include<Factorial.pas>
#define PI 3.14
#define T PI
#define A 12+PI
#define Z PI-PI
#define Y Z-A+13+10+100000+3000+56666+Y
(*#define PI 3.15562;*)
#include<Factorial.pas>

program fibonacci;

function fib(n:integer): integer;
begin
    if(n<=2) then
        fib := 1
    else
        fib := fib(n-1) + fib(n-2);

end;

function circunferencia(r:integer): integer;
begin
	2 * Y *r;
end;



