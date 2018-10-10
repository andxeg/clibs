#!/bin/bash

# BE CAREFULL! Script generates keys and crts in current directory.

# This script generates for each CA from CA_CENTERS array -> CRTs and KEYs.
# Generates for each client from CLIENTS array            -> CRTs and KEYs.
# For i-th client from CLIENTS array his CRT is signed with CA private key
# with name SIGN_ORDER[i] CA.


CA_1="CA_1"
CA_2="CA_2"
TERMINAL="TERMINAL"
SERVER="SERVER"

# ip addresses
IP_TERMINAL="192.168.100.238"
IP_SERVER="192.168.10.203"

CLIENTS=($TERMINAL $SERVER)
DOMAINS=($IP_TERMINAL $IP_SERVER)
CA_CENTERS=($CA_1 $CA_2) # all certification authority centers
SIGN_ORDER=($CA_2 $CA_1) # CRT for CLIENTS[i] will be signed by CA in SIGN_ORDER{i] 

EXT_FILE="v3.ext"

# generate key and crt for CA centers
for i in ${!CA_CENTERS[*]}
do
    center=${CA_CENTERS[$i]}
    openssl genrsa -out $center".KEY" 2048
    openssl req -x509 -new -key $center".KEY" -days 10000 -out $center".CRT"
done

for i in ${!CLIENTS[*]}
do
    printf "\n\n%4d: CLIENT NAME: %s CLIENT ADDRESS: %s (set it to Common Name, not for terminal)\n\n" $i ${CLIENTS[$i]} ${DOMAINS[$i]}

    client=${CLIENTS[$i]}
    domain=${DOMAINS[$i]}
    ca_center=${SIGN_ORDER[$i]}

    # generate private key
    openssl genrsa -out $client".KEY" 2048

    # create certificate signing request
    # set domain or IP in 'Common Name'
    openssl req -new -key $client."KEY" -out $client".CSR" 

    # sign with ca_center private key
    openssl x509 -req -in $client".CSR" -extfile $EXT_FILE -CA $ca_center".CRT" -CAkey $ca_center".KEY" -CAcreateserial -out $client."CRT" -days 5000

    # copy key and crt to one file
    cat $client".KEY" $client".CRT" > $client".PEM"
done

echo "DONE"

