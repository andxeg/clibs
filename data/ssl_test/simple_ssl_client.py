#!/usr/bin/python3

import os
import ssl
import socket


# host_addr = '192.168.10.203'
# host_port = 8443
host_addr = "195.234.190.103" # to remote TMS test
host_port = 5166              # to remote TMS test

# CURR_DIR  = os.path.dirname(os.path.realpath(__file__))
# KEYS_DIR  = os.path.join(CURR_DIR, "keys")
# KEYS_DIR  = os.path.join(KEYS_DIR, "two_ca")

KEYS_DIR = "from_rt" # to remote TMS test
# KEYS_DIR = "analog_rt"
# KEYS_DIR = "mod_analog_rt"
# KEYS_DIR = "special_for_us"
# KEYS_DIR = "tmp_analog"
CLIENT_CRT = os.path.join(KEYS_DIR, "TERMINAL.CRT")
CLIENT_KEY  = os.path.join(KEYS_DIR, "TERMINAL.KEY")
CA_CRT  = os.path.join(KEYS_DIR, "CA.CRT")
# HOSTNAME = "192.168.10.203"
HOSTNAME = "ufa-css02" # fromt_rt, analog_rt, mod_analog, tmp_angalog
# HOSTNAME = "192.168.10.203"
# HOSTNAME = "server-test"

context = ssl.create_default_context(ssl.Purpose.SERVER_AUTH, cafile=CA_CRT)
context.load_cert_chain(certfile=CLIENT_CRT, keyfile=CLIENT_KEY)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
conn = context.wrap_socket(s, server_side=False, server_hostname=HOSTNAME)
conn.connect((host_addr, host_port))
print("SSL established. Peer: {}".format(conn.getpeercert()))
print("Sending: 'Hello, world!")
conn.send(b"Hello, world!")
data = conn.recv(1024)
print("Received: %s" % str(data))
print("Closing connection")
conn.close()
