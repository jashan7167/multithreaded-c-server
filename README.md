# Multithreaded C Server

This repository contains a multithreaded server implemented in C++ that can handle multiple client connections concurrently. The server uses POSIX threads to create a thread pool and processes incoming client requests using multiple threads.

## Features

- Accepts multiple client connections simultaneously
- Uses a fixed-size thread pool to handle client requests efficiently
- Implements a custom queue to manage incoming client connections
- Supports concurrent processing of client requests using threads
- Handles file requests and sends requested files to clients

## Prerequisites

- C++ compiler with C++11 support
- POSIX threads library

## Usage

1. Clone the repository:

   ```bash
   git clone https://github.com/jashan7167/multithreaded-cpp-server.git
Compile the server:

bash
Copy code
g++ server.cpp myqueue.c -o server -lpthread
Run the server:

bash
Copy code
./server
Open a new terminal and run the client script (client.rb) to send requests to the server:

bash
Copy code
ruby client.rb

## Configuration

Server port: Modify the SERVERPORT constant in the server code (server.cpp) to change the listening port for incoming connections.
Thread pool size: Adjust the THREAD_POOL_SIZE constant in the server code to configure the number of threads in the thread pool.

Contributing
Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

