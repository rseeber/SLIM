build:bin/login.o
	g++ main.cpp bin/login.o -lcrypto -o bin/main

bin/login.o:
	g++ -c lib/login.cpp -lssl -lcrypto -o bin/login.o

clean:
	gio trash bin/main