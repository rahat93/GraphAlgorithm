all: prob3

prob3: main.cpp graph1.dat graph2.dat
	g++ -pedantic -Wall -Wextra -ansi main.cpp -o prob3

t3a: prob3
	./prob3 graph1.dat a z
	./prob3 graph1.dat a c
	./prob3 graph1.dat c z
	./prob3 graph1.dat e a

t3b: prob3
	./prob3 graph2.dat a b
	./prob3 graph2.dat b c
	./prob3 graph2.dat c a
