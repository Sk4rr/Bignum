all : main

main : main.o Bignum.o
	g++ -g -o $@ $^

main.o : main.cpp Bignum.hpp
	g++ -g -c $<

Bignum.o : Bignum.cpp Bignum.hpp
	g++ -g -c $<

clean :
	@rm -f *.o main

.PHONY = clean all