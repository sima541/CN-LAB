<!-- Assignment 9 : File Upload and Download using TCP -->

## Assignment 9

### Objective
To understand file upload and download mechanism using **TCP sockets** and to measure the file transfer time between two different hosts in a network.

---

### Exercise
Write a **TCP client-server socket program** in C that allows:

- Downloading a file from server to client.
- Uploading a file from client to server.
- Calculating and displaying the file transfer time for both upload and download operations using `time.h`.

---

### Steps / Hints

1. Create two separate directories in the system where **Mininet** is running:
   - One directory for the **server**
   - One directory for the **client**

2. Open xterm terminals for Mininet hosts and run the client and server programs from their respective directories.

3. Place:
   - A file in the **server directory** for download.
   - Another file in the **client directory** for upload.

4. **File Download**
   - Client connects to the server.
   - Client requests the file.
   - Server sends the file using TCP.
   - Client receives and stores the file in its directory.
   - Transfer time is calculated and displayed on both client and server side.

5. **File Upload**
   - Client sends its file to the server.
   - Server receives and stores the file in its directory.
   - Transfer time is calculated and displayed on both client and server side.

6. Use `time.h` to record start time and end time to calculate the transfer duration.

---

### Files Included

| File Name | Description |
|----------|------------|
| `server.c` | TCP server program for handling file upload and download |
| `client.c` | TCP client program for uploading and downloading files |
| `server_files/` | Directory containing server-side files |
| `client_files/` | Directory containing client-side files |

---

### How to Run in Mininet

#### 1. Start Mininet
```bash
sudo mn --topo=single,2
2. Open xterm terminals
bash
Copy code
xterm h1 h2
3. Directory Setup
On both hosts:

bash
Copy code
mkdir server_files
mkdir client_files
Place a file in server_files for download.

Place another file in client_files for upload.

4. On h1 (Server Host)
bash
Copy code
cd server_files
gcc server.c -o server
./server
5. On h2 (Client Host)
bash
Copy code
cd client_files
gcc client.c -o client
./client 10.0.0.1
Example Output
File Download
yaml
Copy code
Client: Download started...
Client: Download completed.
Time taken: 0.43 seconds

Server: File sent successfully.
Time taken: 0.43 seconds
File Upload
yaml
Copy code
Client: Upload started...
Client: Upload completed.
Time taken: 0.37 seconds

Server: File received successfully.
Time taken: 0.37 seconds
#   Learning Outcomes
Understanding TCP-based file transfer.

Implementation of file upload and download using sockets.

Measuring file transfer time using time.h.

Understanding client-server communication in TCP.

Practical experience with Mininet-based network simulation.

