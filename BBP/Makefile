CC = gcc
CFLAGS = -O0 -std=gnu99 -Wall -fopenmp
OBJ = bench.c BBP.c BBPomp.c

bench:
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lm
plot: bench
	for i in `seq 0 40 10000`; do \
		printf "%d " $$i;\
		./bench $$i;\
	done > out.txt
	gnuplot ./plot.gp

.PHONY: clean
clean:
	-rm bench *.png *.txt