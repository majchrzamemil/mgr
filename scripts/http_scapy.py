#!/usr/bin/env python
from scapy.all import *
import random
import sys

dest = "192.168.1.1"

max = 10

counter = 0
while counter < max:

    sendp(Ether(src="e0:cb:4e:47:34:c0", dst="28:d2:44:07:d5:ac")/IP(src="192.168.1.2", dst="192.168.1.1") / TCP(dport=8080, sport=1234,seq=0,ack= 1, flags='P''A') / "GET /index.html HTTP/1.0 \n\n"), iface="en5s0")
    counter += 1
