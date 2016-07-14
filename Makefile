
filesplitter: streamsplit.h streamsplit.c filesplittermain.c
	gcc -Wall streamsplit.c filesplittermain.c -o filesplitter

example: example.c
	gcc -Wall -o example example.c


server: tcpserver.c
	gcc -o server tcpserver.c

client: tcpclient.c
	gcc -o client tcpclient.c

