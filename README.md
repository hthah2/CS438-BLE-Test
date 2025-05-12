# CS438-BLE-Test
ESP32 BLE Latency Measurement (Client and Server)

# Overview:
This project demonstrates Bluetooth Low Energy (BLE) communication between two ESP32 devices. One ESP32 acts as a BLE server, sending timestamped messages, while the other acts as a BLE client, receiving those messages and calculating the latency based on timestamp comparisons.

# Features:

# BLE Server:

Advertises a custom BLE service and characteristic.

Sends messages with a timestamp using micros().

Transmits data every 3 seconds.

# BLE Client:

Scans for the server named "Hello_BLE".

Connects and subscribes to notifications.

Parses the timestamped message and computes latency in microseconds.

# Hardware Requirements:

Two ESP32 development boards (such as ESP32-WROOM-32)

USB cables

Optional: LED connected to GPIO 25 for activity indication

Getting Started:

Flash the server code to one ESP32.

It will begin advertising a BLE service with UUID: 91bad492-b950-4226-aa2b-4ede9fa42f59.

It sends messages prefixed with "TS:" followed by the microsecond timestamp.

Flash the client code to the second ESP32.

It scans for the server with the name "Hello_BLE".

Once found, it connects and subscribes to the server's characteristic.

The client reads the timestamp, compares it with its current time, and prints the latency.

UUIDs Used:

Service UUID: 91bad492-b950-4226-aa2b-4ede9fa42f59

Characteristic UUID: beb5483e-36e1-4688-b7f5-ea07361b26a8


BLE_client.ino (BLE client code to receive and compute latency)

BLE_server.ino (BLE server code to send timestamped messages)


# Example Serial Output:
Client:
Server found. Connecting!
Connected to server
Latency: 472 µs
Latency: 489 µs

Server:
Waiting for a client...
Sent timestamped message
Sent timestamped message
