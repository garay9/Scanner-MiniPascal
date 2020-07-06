(* vamos
* a incluir el QUicksort*)
#include<algoritmoQuicksort.pas>
program factorial;

{ este es el programa Factorial, es sumamente util}
(* este es otro comentario, siempre feliz, nunca infeliz*)
function fact(n:integer): longint;
begin
	if (n = 0) then fact:=1
	else fact := n * fact(n-1)	
end;
