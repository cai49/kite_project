#!/bin/sh
# launcher.sh
# Navigate to home directory, then to this directory, execute kite_proto.py, the return home

echo "Starting Kite Engine..."
cd /
cd /home/cai49/Documents/Projects/kite_project/rpi5/
sudo python3 kite_proto.py
cd /
echo "Successfully Started Kite Engine"
echo "Waiting for instructions..."