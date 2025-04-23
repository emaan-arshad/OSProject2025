#!/bin/bash

# Kill all mpv instances
pkill mpv

# Remove FIFOs
for i in {0..4}; do
    rm -f fifo_consumer_$i
done

echo "Cleanup complete: All FIFOs removed and mpv instances killed."

