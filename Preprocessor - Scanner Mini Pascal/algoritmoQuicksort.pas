#include<Factorial.pas>
program Quicksort;
 
procedure QuickSort(var AI: array of Integer; ALo, AHi: Integer);
var
  Lo, Hi, Pivot, T: Integer;
begin
  Lo := ALo;
  Hi := AHi;
  Pivot := AI[(Lo + Hi) div 2];
  repeat
    while AI[Lo] <= Pivot do
      Inc(Lo) ;
    while AI[Hi] > Pivot do
      Dec(Hi) ;
    if Lo <= Hi then
    begin
      T := AI[Lo];
      AI[Lo] := AI[Hi];
      AI[Hi] := T;
      Inc(Lo) ;
      Dec(Hi) ;
    end;
  until Lo > Hi;
  if Hi > ALo then
    QuickSort(AI, ALo, Hi) ;
  if Lo < AHi then
    QuickSort(AI, Lo, AHi) ;
end;
 
 
var
  AI: array of Integer;
  I: Integer;
  Len: Integer;
begin
  Write('Array length: ');
  Readln(Len);
  SetLength(AI, Len);
  for I := Low(AI) to High(AI) do
  begin
    Write('AI(',I + 1,') = ');
    Readln(AI[I]);
  end;
 
  QuickSort(AI, Low(AI), High(AI));

end.
