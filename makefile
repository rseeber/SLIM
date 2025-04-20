build:clean bin/login.o
	g++ main.cpp bin/login.o -lcrypto -o bin/main

bin/login.o:
	g++ -c lib/login.cpp -o bin/login.o

clean:
	touch bin/deleteme
	gio trash bin/*