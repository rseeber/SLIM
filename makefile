build:
	g++ main.cpp lib/login.cpp -lssl -lcrypto -o bin/main

run:
	bin/main

clean:
	gio trash bin/main