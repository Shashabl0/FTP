server:
	wsl gcc server.c -o server

client:
	wsl gcc client.c -o client

clean:
	rm -f server client

update: clean server client