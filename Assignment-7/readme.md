Objective

To understand the reliability issues of UDP communication and develop a scientific calculator application that works over UDP, while detecting possible packet loss.

Exercise

Write a UDP client–server program where:

Client sends:

1 or 2 numbers

A scientific operation (e.g., sin, cos, *, /, inv, etc.)

Server responds with:

The result of the operation

Any error messages (e.g., division by zero)

Extra Requirement:
Detect (using Wireshark or sequence numbers) if any UDP packet loss happens.

Example client message:

sin 30


Server returns:

Result = 0.5


Another example:

10 * 5


Server returns:

Result = 50

Steps / Hints

Use UDP sockets ?

socket(AF_INET, SOCK_DGRAM, 0);


Server receives input using:

recvfrom()


Client sends using:

sendto()


Server must use math.h functions such as:

sin()

cos()

sqrt()

pow()

Add simple sequence numbers or rely on Wireshark to detect packet loss.

Wireshark filter:

udp

Files Included

udp_calc_server.c — UDP scientific calculator server

udp_calc_client.c — UDP client that sends operations and numbers

Wireshark .pcap file showing captured packets

Optional: text file showing any packet loss analysis

How to Run in Mininet
1. Start Mininet
sudo mn --topo=single,2

2. Open host terminals
xterm h1 h2

3. On h1 (Server Host)

Compile & run server:

gcc udp_calc_server.c -o udp_calc_server -lm
./udp_calc_server

4. On h2 (Client Host)

Compile & run client:

gcc udp_calc_client.c -o udp_calc_client -lm
./udp_calc_client 10.0.0.1

Example Client Input
sin 45


Server Response:

0.7071


Client Input:

10 / 0


Server Response:

Error: Division by zero


Client Input:

4 ^ 3


Server Response:

64

Packet Loss Detection
Method 1: Using Wireshark

Start Wireshark on host h1 (listening on h1-eth0).

Apply filter:

udp


Check if any packet is missing from the sequence.

Method 2: Sequence Numbers

Client sends messages like:

#1 sin 30
#2 10 * 5
#3 cos 60


Server prints received sequence, detects missing packets.

Learning Outcomes

Understanding UDP unreliability (no ACK, no retransmission).

Using UDP sockets with mathematical function processing.

Ability to send structured expressions over the network.

Detecting and analyzing packet loss using Wireshark.

Hands-on practice with scientific operations in C (math.h).


