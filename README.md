# splitproxy
Reverse TCP proxy server for Linux, used for piping requests and responses to a secondary server without copying data into user space.

## usage

`splitproxy <port> <remotehost:port> <logginghost:port>`

ex: `splitproxy 80 10.0.1.1:80 10.0.1.2:80` listens on port 80, tees the incoming request to 10.0.1.1:80 and 10.0.1.2:80, then tees the response to the client and 10.0.1.2:80.

## testing

`make test` creates 

1. `testclient <host:port>` - a client that pipes stdin to a socket with connected to <host:port>
2. `testserver <port>` which listens on <port> and pipes socket data to stdout

`./test/test.sh`

1. pipes data from `./test/in.txt` to `tcpclient`
2. `tcpclient` connects to `splitproxy`
3. `splitproxy` connects to two `testserver`s and streams the proxy's incoming data to the `testservers`
4. diffs the files and prints either the diff, or a success message if the diff is empty.

