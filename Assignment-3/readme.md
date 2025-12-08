<!-- Write a UDP client–server socket program where:

Client sends a fruit name and quantity using UDP. -->

Server responds with:

The price of the fruit

Whether the quantity is available

The total cost

This is the UDP version of Assignment-2.

Steps / Hints

Use UDP sockets – i.e., socket(AF_INET, SOCK_DGRAM, 0)

No listen() or accept() needed because UDP is connectionless.

Use:

sendto() at client

recvfrom() at server

Server maintains a list of fruits with prices.

Client sends: "Apple 2"

Server replies: "Price: 30 each, Total = 60, Available"

Files Included

udp_fruit_server.c → UDP server that:

Receives fruit + quantity via recvfrom()

Computes price

Sends response back using sendto()

udp_fruit_client.c → UDP client that:

Takes fruit name + quantity as input

Sends request to server

Displays server's reply

How to Run in Mininet
1. Start Mininet with 2 hosts
sudo mn --topo=single,2

2. Open both hosts in xterms
xterm h1 h2

3. On h1 (Server Host)

Compile and run:

gcc udp_fruit_server.c -o udp_fruit_server
./udp_fruit_server

4. On h2 (Client Host)

Compile and run:

gcc udp_fruit_client.c -o udp_fruit_client
./udp_fruit_client 10.0.0.1

Example Session

Client Input:

Banana 5


Server Reply (displayed on client):

Banana price = Rs. 5 each
Total = Rs. 25
Available

<!-- Learning Outcomes -->

Understanding UDP vs TCP differences (unreliable, connectionless).

Using sendto() and recvfrom() for communication.

Observing that UDP has no connection setup, no error control.

Designing and executing small application-layer protocols over UDP.