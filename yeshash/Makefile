	# link de bibliotecas utilizadas
	TESTLINKER=-lcrypto
	# definindo o compilador utilizado
	CC = gcc
	# definindo arquivo fonte
    	TESTSOURCES=t.c
	# definindo o nome do executavel
    	TESTBINARY=t
	#definindo uma variavel
	TESTMSG=oi

.FORCE: 
	test

all:
	make test

clean:
	rm -rf *.o $(TESTBINARY)

test:
	$(CC) $(TESTSOURCES) -o $(TESTBINARY) $(TESTLINKER) 
	./$(TESTBINARY) $(TESTMSG)

