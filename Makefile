CC = clang
CFlAGS = -Wall -Werror -Wextra -Wpedantic -gdwarf-4 

banhammer: banhammer.o bv.o bf.o city.o node.o ll.o ht.o parser.o strfuncs.o
	$(CC) -o $@ $^



%.o: %.c
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -f hash *.o 

format:
	clang-format -i -style=file banhammer.c parser.c ht.c ll.c node.c bf.c bv.c

spotless:
	rm -f $(EXECBIN) *.o
