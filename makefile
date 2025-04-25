pkg:build
	cp login.hpp bin/login.hpp
	echo "Remember to 'gpg --output bin/login.o.sig --detach-sig bin/login.o' to sign the binary!"

build:clean bin/login.o
	g++ example/main.cpp bin/login.o -lcrypto -o bin/main

bin/login.o:
	g++ -c login.cpp -o bin/login.o

clean:
	gio trash bin/login.o -f
	gio trash bin/main -f