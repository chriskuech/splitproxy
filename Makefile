

default: test


##
# splitproxy
#

splitproxy: ./main.c ./lib/streamsplit.h ./lib/streamsplit.c ./lib/networking.h ./lib/networking.c
	gcc -o splitproxy -pthread ./main.c ./lib/streamsplit.c ./lib/networking.c


##
# tests
#

testclient: splitproxy ./test/testclient.c ./lib/networking.h ./lib/networking.c
	gcc -o testclient -pthread ./test/testclient.c ./lib/networking.c

testserver: splitproxy ./test/testserver.c ./lib/networking.h ./lib/networking.c
	gcc -o testserver -pthread ./test/testserver.c ./lib/networking.c

test: testclient testserver splitproxy


##
# clean
#
clean:
	rm ./testclient ./testserver ./splitproxy ./test/out1.txt ./test/out2.txt

