#!/bin/bash

# Create FIFOs
for i in {0..4}; do
    mkfifo fifo_consumer_$i
done

# Launch mpv players (one per consumer)
for i in {0..4}; do
    mpv --loop --idle --no-terminal --input-ipc-server=/tmp/mpv_socket_$i fifo_consumer_$i &
done

# Compile and run the video server
gcc -o video_server video_server.c -lpthread
./video_server

echo "Server exited. To clean up FIFOs, run: ./cleanup.sh"
