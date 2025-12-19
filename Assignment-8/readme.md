<!-- Objective -->

To understand multi-threading with TCP sockets and implement a group chat server where multiple clients communicate with each other in real time. The server must also maintain a message log.

<!-- Exercise -->

Write a C program using pthread thread library + TCP sockets that:

Accepts multiple clients simultaneously.

Creates one thread per client using pthread_create().

Allows all clients to chat with each other through the server (like a group chat).

Maintains a log.txt file where all messages are saved with timestamps.

Every message received from any client must be broadcast to all connected clients.

<!-- Steps / Hints -->

Use the standard TCP server flow:

socket()

bind()

listen()

accept()

For each accepted client:

pthread_create(&thread_id, NULL, client_handler, (void*) &newfd);


<!-- Server maintains: -->

A global array of client sockets

A mutex lock for thread-safe logging and broadcasting

Each thread:

Receives messages from a specific client

<!-- Writes them to log.txt -->

Sends them to all other connected clients

Client program:

Connects to server

Sends and receives messages continuously

Files Included

server.c - Multithreaded TCP server program

client.c - Client program to send and receive messages

log.txt - Log file containing full chat history with timestamps

<!-- How to Run in Mininet -->
1. Start Mininet
sudo mn --topo=single,3


(At least 3 hosts recommended for group chat.)

2. Open xterm terminals
xterm h1 h2 h3

3. On h1 (Chat Server Host)

Compile and run:

gcc server.c -o server -lpthread
./server

4. On h2 and h3 (Clients)

Compile and run:

gcc client.c -o client
./client 10.0.0.1

Example Session
Client h2:
Hi everyone!

Client h3 receives:
h2: Hi everyone!

Server log.txt:
[12:10:35] h2: Hi everyone!

Client h3 sends:
Hello h2!

Client h2 receives:
h3: Hello h2!

<!-- Learning Outcomes -->

Understanding multi-threading in C using pthread.

Creating and synchronizing threads for multiple clients.

Managing shared resources via mutex locks.

Implementing real-time chat using TCP sockets.

Maintaining server-side logs with timestamps.

Understanding how chat applications work internally
