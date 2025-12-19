<!-- Assignment 10 : Packet Generation using RAW Sockets -->

## Assignment 10

### Objective
To understand the use of **RAW sockets** for generating custom network packets and to create:
1. A TCP packet with user-defined header and payload.
2. ICMP Timestamp messages sent to a target IP and analyze them using Wireshark.

---

### Exercise
Write two C programs using **RAW sockets** that:

1. Generate and send a **custom TCP packet** where the TCP payload contains the **student roll number**.
2. Generate and send **ICMP Timestamp request messages** towards a target IP address.
3. Capture and analyze the generated packets at the receiver side using **Wireshark**.

---

### Steps / Hints

#### 1. TCP Packet Generation using RAW Socket
- Create a RAW socket using:
  ```c
  socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
Manually construct:

IP header

TCP header

TCP payload (containing roll number)

Calculate and fill:

IP checksum

TCP checksum

Send the packet to the target IP using sendto().

2. ICMP Timestamp Message Generation
Use RAW socket with ICMP protocol:

c
Copy code
socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
Use ICMP Timestamp Request message:

ICMP Type: 13

ICMP Code: 0

Fill the ICMP timestamp fields:

Originate Timestamp

Receive Timestamp

Transmit Timestamp

Send ICMP timestamp packets to the target IP.

3. Packet Analysis using Wireshark
Run Wireshark on the receiver host.

Apply filters:

For TCP:


tcp
For ICMP Timestamp:


icmp.type == 13 || icmp.type == 14
Verify:

TCP payload contains the roll number.

ICMP timestamp request and reply packets are visible.

Files Included
File Name	Description
raw_tcp.c	C program to generate custom TCP packet using RAW socket
raw_icmp.c	C program to generate ICMP Timestamp messages
README.md	Assignment documentation

How to Run in Mininet
1. Start Mininet
bash
Copy code
sudo mn --topo=single,2
2. Open xterm terminals
bash
Copy code
xterm h1 h2
3. On h1 (Sender Host)
Compile and run TCP RAW packet program:

gcc raw_tcp.c -o raw_tcp
sudo ./raw_tcp 10.0.0.2
Compile and run ICMP Timestamp program:


gcc raw_icmp.c -o raw_icmp
sudo ./raw_icmp 10.0.0.2
4. On h2 (Receiver Host)
Start Wireshark:


wireshark &
Apply ICMP or TCP filter and observe incoming packets.

<!-- Example Observation -->
TCP packet captured in Wireshark shows:

Custom TCP header fields.

Payload containing roll number.

ICMP Timestamp packets captured:

ICMP Type 13 (Timestamp Request)

ICMP Type 14 (Timestamp Reply)

<!-- Learning Outcomes -->
Understanding RAW sockets and their use.

Creating custom TCP packets with user-defined payload.

Generating ICMP Timestamp messages.

Manual construction of protocol headers.

Packet analysis using Wireshark.

Understanding low-level packet generation and networking internals.