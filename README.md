 gcc -lm seq.c -o seq
e
 gen.py auto && seq auto > out && t.py out
 
ou

 mpicc -lm par.c -o par
e
 gen.py auto && mpirun par auto > out && t.py out
