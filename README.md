seq -> sequencial

par -> paralelo 
 
compilar com flag -lm 
___ 
programa [file]
___
formato de arquivo: (e.g. in)

numero de planetas 

massa x y 

massa x y
...

___
t.py 

le o output do programa e anima

gen.py

gera arquivos
___
e.g.

 gcc seq.c -lm -o seq && seq in > b && t.py b

 gen.py auto && seq auto > b && t.py b
