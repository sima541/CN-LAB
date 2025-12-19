<!-- Assignment 13 : Packet Capturing and Analysis -->

## Assignment 13

### Objective
To learn packet capturing and packet analysis by creating a binary tree topology in Mininet, capturing packets at the root switch, extracting protocol headers from a captured file, and drawing a time diagram for packets observed during a PING operation.

---

### Exercise
Create a binary tree topology with **7 switches** in Mininet. Capture packets at the **root switch** during a PING operation. Save the captured packets in a `.pcap/.pcapng` file and write a C program to extract protocol headers and draw a **time diagram** showing protocol occurrences. Also list the L2, L3, and L4 protocols extracted from the capture file.

---

### Steps / Procedure

#### 1. Create Mininet Topology
Create a binary tree topology with 7 switches using the following command:
```bash
sudo mn --topo=tree,3 --controller=none
2. Packet Capture at Root Switch
Open xterm for the root switch:


xterm s1
Start packet capture at the root switch:


tcpdump -i s1-eth1 -w ping_capture.pcap
3. Generate Traffic
From any host in Mininet:<!-- Assignment 14 : Custom Leaf-Spine Topology in Mininet -->

## Assignment 14

### Objective
To understand the **Mininet Python API** by creating a **custom Leaf–Spine topology** that can be scaled by increasing the switch radix.

---

### Exercise
Create a **custom Leaf-Spine topology** in Mininet using **Python**, where:
- Leaf switches connect to hosts.
- Spine switches interconnect all leaf switches.
- The topology can be easily scaled by changing parameters such as number of leaf switches, spine switches, and hosts per leaf.
- A **fat-tree topology** may be used as a reference.

---

### Steps / Procedure

#### 1. Writing Custom Topology Code
- Use Mininet’s Python API.
- Create a custom topology class by extending the `Topo` class.
- Add:
  - Spine switches
  - Leaf switches
  - Hosts
- Connect:
  - Each leaf switch to all spine switches.
  - Hosts to leaf switches.

---

#### 2. Running the Custom Topology
- Save the Python file (for example: `leaf_spine.py`).
- Run the topology using:
```bash
sudo python3 leaf_spine.py
3. Verifying the Topology
Use Mininet CLI commands:

bash
Copy code
net
pingall
Ensure connectivity between all hosts.

Example Leaf–Spine Structure
less
Copy code
        Spine1     Spine2
          |  \     /  |
          |   \   /   |
        Leaf1   Leaf2   Leaf3
         | |     | |     | |
        h1 h2   h3 h4   h5 h6
Spine switches provide high-bandwidth interconnection.

Leaf switches connect directly to hosts.

Architecture is scalable and commonly used in data centers.

Files Included
File Name	Description
leaf_spine.py	Python script implementing custom Leaf-Spine topology
README.md	Assignment documentation

Learning Outcomes
Understanding Mininet Python API.

Creating custom and scalable network topologies.

Learning Leaf-Spine architecture used in data centers.

Understanding topology scalability using switch radix.

Gaining practical experience with virtual network experiments.




h1 ping -c 5 h8
Stop packet capture using Ctrl + C.
The packet capture file ping_capture.pcap is generated.

4. Packet Analysis
Analyze the .pcap file using Wireshark or a C program based on libpcap.

Extract packet headers and timestamps.

Identify protocol types and draw a time diagram showing packet sequence during PING.

Time Diagram (PING Operation)
mathematica

Time →
t1  ARP Request
t2  ARP Reply
t3  ICMP Echo Request
t4  ICMP Echo Reply
t5  ICMP Echo Request
t6  ICMP Echo Reply
Protocols Extracted from Packet Capture
Layer 2 (L2)
Ethernet

Layer 3 (L3)
ARP

IP

Layer 4 (L4)
ICMP

Files Included
File Name	Description
ping_capture.pcap	Captured packets at root switch
pcap_analyzer.c	C program to extract headers and timestamps
README.md	Assignment documentation

#  Learning Outcomes
Understanding packet capturing using tcpdump and Wireshark.

Learning packet analysis from .pcap files.

Identifying L2, L3, and L4 protocols in network communication.

Understanding protocol behavior during PING operation.

Practical experience with Mininet-based packet analysis.