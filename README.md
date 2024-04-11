# Chat Room using C++ and Windows APIs

This project implements a simple chat room application using C++, Windows APIs, and socket programming with TCP protocol. The application allows multiple users to connect to a central server and exchange messages in real-time.

# Features

.Server-Client Architecture: The chat room consists of a central server and multiple clients. Clients connect to the server to send and receive messages.

.Windows APIs: The project utilizes Windows APIs for creating the graphical user interface (GUI) and handling networking operations.

.Socket Programming: Communication between the server and clients is implemented using socket programming. TCP protocol is used for reliable data transmission.

# Getting Started

To run the chat room application, follow these steps:


.Compile the Code: Use a C++ compiler compatible with Windows APIs to compile the server and client source code files.

.Start the Server: Run the compiled server executable on a machine that acts as the central server. Make sure to specify the port number on which the server will listen for incoming connections.

.Connect Clients: Execute the compiled client executable on multiple machines to connect them to the chat room. Provide the IP address and port number of the server to establish a connection.

.Start Chatting: Once the clients are connected, they can start exchanging messages in the chat room. Messages sent by one client will be broadcasted to all other connected clients by the server.

# Code Structure

The project consists of the following main components:


.Server Source Code: Contains the implementation of the server application, including socket creation, connection handling, and message broadcasting.

.Client Source Code: Implements the client-side functionality, including connecting to the server, sending messages, and receiving messages from other clients.

# Contributing

Contributions to this project are welcome. If you have suggestions for new features, improvements, or bug fixes, feel free to open an issue or submit a pull request.
