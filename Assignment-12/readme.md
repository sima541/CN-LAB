<!-- Assignment 12 : ICMP Flooding Attack using RAW Sockets -->

## Assignment 12

### Objective
To understand **ICMP flooding based DDoS attacks** by generating ICMP packets using **RAW sockets**, and to analyze ICMP communication patterns during such attacks.

---

### Exercise
Perform the same attack scenario as **Assignment 11**, but instead of TCP SYN packets, use **ICMP packets** generated through **RAW sockets** to simulate an **ICMP flooding attack** towards a target IP address.

---

### Network Setup
A total of **six Mininet hosts** are used:

- **1 Attacker Host** – Generates ICMP flood packets.
- **1 Victim Host** – Receives a large number of ICMP packets.
- **4 Spoofed Agent Hosts** – Simulate distributed sources using spoofed IP addresses.

This setup represents a **Distributed Denial of Service (DDoS)** attack scenario using ICMP.

---

### Steps / Hints

#### 1. RAW Socket Creation
- Create a RAW socket using ICMP protocol:
  ```c
  socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
Enable manual construction of ICMP packets.

2. ICMP Packet Construction
Manually create:

IP header

ICMP header

Use suitable ICMP message type:

ICMP Echo Request (Type 8) for flooding

Fill ICMP fields:

Checksum

Identifier and sequence number

Optionally spoof source IP addresses to simulate multiple agent devices.

3. ICMP Flooding Mechanism
Send ICMP packets continuously in a loop using sendto().

High rate of ICMP requests causes:

Excessive ICMP replies

Increased CPU and network load on the victim host.

4. Mininet Execution
Create Mininet topology with six hosts.

Run the ICMP flooding program from the attacker host.

Monitor the victim host using Wireshark to observe ICMP traffic.

Files Included
File Name	Description
raw_icmp_flood.c	RAW socket program for generating ICMP flooding attack
README.md	Assignment documentation

<!-- How to Run in Mininet -->
1. Start Mininet
bash
Copy code
sudo mn --topo=single,6
2. Open xterm terminals
bash
Copy code
xterm h1 h2 h3 h4 h5 h6
h1 → Attacker

h2 → Victim

h3–h6 → Spoofed agents

3. On Attacker Host
Compile and run:

gcc raw_icmp_flood.c -o icmp_flood
sudo ./icmp_flood 10.0.0.2
4. On Victim Host
Start Wireshark:


wireshark &
Apply filter:


icmp
Observation
Victim receives a large number of ICMP Echo Request packets.

ICMP packets appear to originate from multiple spoofed IP addresses.

Network congestion and high ICMP traffic are observed.

Victim system performance may degrade under heavy flooding.

<!-- Learning Outcomes -->
Understanding RAW sockets for ICMP packet generation.

Creation of custom ICMP packets using user-defined headers.

Simulation of ICMP flooding based DDoS attack.

Understanding ICMP communication patterns during flooding.

Packet analysis using Wireshark.

Practical exposure to network-layer DDoS attacks.

