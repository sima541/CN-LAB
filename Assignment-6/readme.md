Objective

To gain knowledge of low-level TCP/IP C libraries and learn how to access, read, and analyze raw TCP packets received at a network interface.

Exercise

Develop a simple C-based program that:

Creates a TCP socket and listens for incoming packets.

Extracts header fields (like source/destination IP, ports) from the received packets.

Extracts payload data from the incoming TCP segments.

Prints header + data information on the terminal.

This simulates a very basic network packet analyzer.

Steps / Hints

Use TCP/IP C functions like:

socket()

bind()

listen()

accept()

recv()

Use struct sockaddr_in to extract:

Client IP

Port numbers

Use C structs for parsing headers:

struct iphdr (IP header)

struct tcphdr (TCP header)

Print:

Source IP

Destination IP

Source port

Destination port

Data length

Payload data (if printable)

Files Included

tcp_sniffer.c – A C program that:

Listens on a TCP socket

Receives incoming packets

Extracts and prints header + data info

Output file / terminal log showing packet details.

How to Run in Mininet
1. Start Mininet
sudo mn --topo=single,2

2. Open two terminals
xterm h1 h2

3. On h1 (Packet Sniffer / Server)

Compile + run:

gcc tcp_sniffer.c -o tcp_sniffer
./tcp_sniffer


It will wait for incoming TCP data.

4. On h2 (Client sending TCP packets)

Run a normal TCP client program to send test data:

echo "Hello Network" | nc 10.0.0.1 9000


Or your custom TCP client:

./tcpclient 10.0.0.1

Expected Output (Example)

On h1 (tcp_sniffer output):

Received a TCP packet!
Source IP: 10.0.0.2
Source Port: 53214
Destination Port: 9000
Payload Length: 13 bytes
Payload: Hello Network

Learning Outcomes

Understanding of TCP/IP header structures in C.

Ability to use socket programming to receive and inspect packets.

Basic knowledge of writing a custom packet analyzer.

Extracting and printing protocol fields using C structs.

Hands-on experience with networking at a lower level.
