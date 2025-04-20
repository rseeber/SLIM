build:clean bin/login.o
	g++ main.cpp bin/login.o -lcrypto -o bin/main

bin/login.o:
	g++ -c lib/login.cpp -o bin/login.o

clean:
	gio trash bin/login.o -f
	gio trash bin/main -f