server:
	wsl gcc server.c -o server

client:
	wsl gcc client.c -o client

clean:
	rm -f server client

all: server client

update: clean all