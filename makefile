pkg:build
	# copy all necessary header files into the package output folder
	cp login.hpp bin/login.hpp		# login.hpp for userspace function definitions
	cp config.hpp bin/config.hpp	# config.hpp for userspace configurations
	@printf "\n\nRemember to 'gpg --output bin/login.o.sig --detach-sig bin/login.o' to sign the binary!\n"

build:clean bin/login.o
	g++ example/main.cpp bin/login.o -lcrypto -o bin/main

bin/login.o:
	# Create the bin/ folder, since 'make clean' deletes it
	mkdir bin
	g++ -c login.cpp -o bin/login.o

clean:
	gio trash bin/ -f