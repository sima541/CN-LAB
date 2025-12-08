Objective

To gain first-hand experience with basic TCP Socket Programming and understand how a client and server establish a connection and exchange simple messages.

Exercise

Write a TCP client–server program where:

Client sends ? "Hi"

Server replies ? "Hello"

This demonstrates basic socket creation, binding, listening, accepting, connecting, sending, and receiving operations.

Steps / Hints

Create two Mininet hosts.

Open both hosts individually using xterm.

Run the TCP server on one host, then run the TCP client on the other host.

Files Included

tcpserver.c ? Minimal TCP server program

tcpclient.c ? Minimal TCP client program

How to Run in Mininet
1. Start Mininet with 2 hosts
sudo mn --topo=single,2

2. Open xterms for both hosts
xterm h1 h2

3. On h1 (Server Host)

Compile & run server:

gcc tcpserver.c -o tcpserver
./tcpserver

4. On h2 (Client Host)

Compile client & connect to h1 (server IP = 10.0.0.1):

gcc tcpclient.c -o tcpclient
./tcpclient 10.0.0.1


Once connected:

Client prints ? "Server says: Hello"

Server prints ? "Client says: Hi"

Learning Outcomes

Understanding of TCP socket lifecycle (socket ? bind ? listen ? accept).

Ability to create TCP client and server.

Ability to exchange messages using send() and recv().

Experience running real network programs inside Mininet.
