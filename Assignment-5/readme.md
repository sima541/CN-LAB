<!-- Objective -->

To gain knowledge of different packet generation tools and understand how they are used to create traffic patterns for network testing and analysis.

<!-- Exercise -->

Install and use as many traffic generation tools as possible (such as iPerf, D-ITG, etc.) and send traffic between Mininet hosts.
Analyze the generated traffic using Wireshark or command-line outputs.

Steps / Hints
1. Install traffic generation tools

Install iPerf:

sudo apt install iperf


Install D-ITG:

sudo apt install ditg


(Note: Some systems use ditg-gen, ITGSend, ITGRecv commands.)

2. Start Mininet
sudo mn --topo=single,2

3. Open host terminals
xterm h1 h2

Using iPerf
A. On h1 ? Start iPerf Server
iperf -s

B. On h2 ? Run iPerf Client

Send TCP traffic:

iperf -c 10.0.0.1 -t 10


Send UDP traffic:

iperf -c 10.0.0.1 -u -b 10M -t 10

Output shows:

Bandwidth

Throughput

Jitter (UDP)

Loss rate

Using D-ITG
A. On h1 (Receiver)
ITGRecv

B. On h2 (Sender)

Send TCP traffic:

ITGSend -a 10.0.0.1 -T TCP -C 1000 -t 10000


Send UDP traffic:

ITGSend -a 10.0.0.1 -T UDP -C 800 -t 10000


After sending, generate report:

ITGDec recv_log_file

Files Included

iPerf test logs

D-ITG test logs

Optional .pcap file captured using Wireshark

What to Observe
iPerf

TCP/UDP throughput

Bandwidth over time

Packet loss (UDP)

D-ITG

Inter-arrival time

Packet delay

Jitter

Loss

Bandwidth

Learning Outcomes

Understanding of traffic generation tools used in networking research.

Ability to simulate bandwidth-heavy applications.

Compare TCP vs UDP performance.

Learn how packet generators affect delay, jitter, and throughput.

//Ability to analyze generated traffic using Wireshark or tool logs.
