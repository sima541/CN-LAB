

### Exercise

<!-- Install and run Wireshark inside your Mininet VM.

Capture packets while performing a PING between two Mininet hosts.

Open the .pcap file in Wireshark and:

Draw a time diagram showing ICMP Request & Reply

Identify Ethernet (L2), IP (L3), and ICMP (L4) header fields

Observe how many packets were transmitted/received during ping. -->


### Objective

<!-- To gain knowledge of packet capturing tools and understand the structure of L2, L3 and L4 protocol headers using Wireshark. -->


Steps / Hints
1. Start Mininet with 2 hosts
sudo mn --topo=single,2

2. Open xterm windows
xterm h1 h2

3. Start Wireshark

From Ubuntu terminal:

sudo wireshark


Select interface: h1-eth0 or h2-eth0

4. Start capturing packets

Click on the interface → Start Capture

5. Generate traffic using ping

Run on h1:

ping 10.0.0.2


You will now see packets in Wireshark:

ICMP Echo Request (ping request)

ICMP Echo Reply (ping response)

6. Save capture

File → Save As → ping_capture.pcapng

7. Analyse headers

Expand the header sections in Wireshark:

🔹 L2 – Ethernet Header

Destination MAC

Source MAC

EtherType = 0x0800 (IPv4) or 0x86DD (IPv6)

🔹 L3 – IP Header

Version (4 or 6)

Source IP (ex: 10.0.0.1)

Destination IP (ex: 10.0.0.2)

TTL

Protocol = 1 (ICMP)

Header length

Total length

🔹 L4 – ICMP Header

Type (8 = Request, 0 = Reply)

Code

Checksum

Identifier

Sequence Number

Time Diagram to Include in Report
h1 ---------------- ICMP Echo Request ----------------→ h2
h1 ←--------------- ICMP Echo Reply ------------------- h2


or more detailed:

t0: h1 → h2 : ICMP Echo Request (Type 8)
t1: h2 → h1 : ICMP Echo Reply   (Type 0)


This diagram shows how ICMP works in a simple ping operation.

<!-- Learning Outcomes -->

Ability to use Wireshark for packet capture & analysis.

Understanding of how Ethernet, IP, and ICMP headers are structured.

Visualizing packet flow using time diagrams.

Observing the behavior of ICMP Echo Request & Reply packets.

Experience working with .pcap files.