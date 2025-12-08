## Objective

To understand TCP client–server communication involving multiple inputs and server-side computation/decision making.

## Exercise

Write a TCP client–server socket program where:

1. Client sends the name of a fruit and the quantity required.

2. Server replies with:
Its price,
 Whether sufficient stock is available,

Total cost.

Example:
Client sends ? "Mango 2"
Server responds ? "Rs. 40, Available"
Server internally maintains fruit prices.

Steps / Hints

Run Mininet with two hosts.

Open two terminals with xterm h1 h2.

Server should maintain a small fruit-price list (e.g., Apple, Mango, Banana).

Client enters fruit name + quantity and sends to server.

Server processes input and returns result to client.

Files Included

fruit_server.c ? TCP server that:

Stores fruit names + prices

Reads client request

Computes total price

Sends response back

fruit_client.c ? TCP client that:

Accepts user input

Sends it to server

Displays server’s response

How to Run in Mininet
1. Start Mininet with 2 hosts
sudo mn --topo=single,2

2. Open xterms
xterm h1 h2

3. On h1 (Server Host)

Compile and run server:

gcc fruit_server.c -o fruit_server
./fruit_server

4. On h2 (Client Host)

Compile and run client:

gcc fruit_client.c -o fruit_client
./fruit_client 10.0.0.1

Example Session

Client input:

Mango 3


Server output (to client):

Mango price: 20 per piece
Total: Rs. 60
Available


Server terminal also logs:

Client requested Mango 3

Learning Outcomes

Application-level protocol design (client request + server reply).

Using TCP to transmit structured data (strings + numbers).

Server-side parsing of client messages.

Basic inventory/price computation over network.

Running applications on Mininet hosts.
