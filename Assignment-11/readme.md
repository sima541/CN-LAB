<!-- Assignment 11 : TCP SYN Flooding Attack using RAW Sockets -->

## Assignment 11

### Objective
To understand **TCP SYN flooding based DDoS attacks** by generating custom TCP SYN packets using **RAW sockets**, and to analyze the behavior of TCP control messages during such attacks.

---

### Exercise
Write a **RAW socket program** in C to generate a **TCP SYN flood attack** towards a target IP address.  
The attack should be simulated using **Mininet**, where multiple hosts act as spoofed agent devices.

---

### Network Setup
A total of **six Mininet hosts** are used:

- **1 Attacker Host** – Generates TCP SYN flood packets.
- **1 Victim Host** – Receives large number of SYN packets.
- **4 Spoofed Agent Hosts** – Used to simulate distributed sources (spoofed IPs).

This setup demonstrates a **Distributed Denial of Service (DDoS)** scenario.

---

### Steps / Hints

#### 1. RAW Socket Creation
- Create a RAW socket using:
  ```c
  socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
Enable manual construction of packet headers.

2. TCP SYN Packet Construction
Manually create:

IP header

TCP header

Set TCP flags:

SYN flag = 1

ACK, FIN, RST flags = 0

Use random or spoofed source IP addresses to simulate multiple agents.

Set destination IP as the victim host.

3. Flooding Mechanism
Send TCP SYN packets repeatedly in a loop using sendto().

High packet rate causes:

Half-open TCP connections at victim.

Consumption of victim resources.

4. Mininet Execution
Create Mininet topology with six hosts.

Run the attack program from the attacker host.

Observe victim behavior using:

<!-- Terminal output -->

Wireshark packet capture

Files Included
File Name	Description
raw_syn_flood.c	RAW socket program for generating TCP SYN flood
README.md	Assignment documentation

How to Run in Mininet
1. Start Mininet

sudo mn --topo=single,6
2. Open xterm terminals

xterm h1 h2 h3 h4 h5 h6
h1 → Attacker

h2 → Victim

h3–h6 → Spoofed agents

3. On Attacker Host

gcc raw_syn_flood.c -o syn_flood
sudo ./syn_flood 10.0.0.2
4. On Victim Host
Run Wireshark:


wireshark &
Apply filter:


tcp.flags.syn == 1 && tcp.flags.ack == 0
Observation
Victim receives a large number of TCP SYN packets.

Multiple half-open TCP connections are observed.

TCP SYN packets appear to originate from multiple spoofed IP addresses.

Victim may experience performance degradation.

<!-- Learning Outcomes -->
Understanding RAW sockets and low-level packet generation.

Creation of custom TCP SYN packets.

Simulation of SYN flood based DDoS attack.

Understanding TCP three-way handshake abuse.

Analysis of TCP control messages using Wireshark.

Practical exposure to network security attack patterns.