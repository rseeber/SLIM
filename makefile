build:
	g++ main.cpp lib/login.cpp -lssl -lcrypto -o bin/main

obj:
	g++ -c lib/login.cpp -lssl -lcrypto -o pkg/main.o

buildLib:
	g++ main.cpp pkg/main.o -lcrypto -o bin/main

run:
	bin/main

clean:
	gio trash bin/main