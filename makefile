program:
	gcc -Wall -o executa_postergado executa_postergado.c
	gcc -Wall -o escalonador escalonador.c
	gcc -Wall -o gerente_execucao gerente_execucao.c

executa_postergado.o: executa_postergado.c
	gcc -Wall -o executa_postergado executa_postergado.c
escalonador.o: escalonador.c
	gcc -Wall -o escalonador escalonador.c
gerente_execucao.o: gerente_execucao.c
	gcc -Wall -o gerente_execucao gerente_execucao.c



test:
	gcc -Wall -o hello hello.c
	gcc -Wall -o hello2 hello2.c
	gcc -Wall -o hello3 hello3.c
	./executa_postergado hello 10
	./executa_postergado hello2 3
	./executa_postergado hello3 6

hypercube:
	./escalonador hypercube &

torus:
	./escalonador torus &

fat_tree:
	./escalonador fat_tree &