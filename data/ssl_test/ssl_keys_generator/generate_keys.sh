#!/bin/bash

# key names
CA="CA"
TERMINAL="TERMINAL"
SERVER="SERVER"

# ip addresses
IP_TERMINAL="192.168.100.238"
IP_SERVER="192.168.10.203"

CLIENTS=($TERMINAL $SERVER)
DOMAINS=($IP_TERMINAL $IP_SERVER)

EXT_FILE="v3.ext"

# generate key and crt for CA
openssl genrsa -out $CA".KEY" 2048
openssl req -x509 -new -key $CA".KEY" -days 10000 -out $CA".CRT"


# generate key and crt for clients
for i in ${!CLIENTS[*]}
do
    printf "\n\n%4d: CLIENT NAME: %s CLIENT ADDRESS: %s (set it to Common Name, not for terminal)\n\n" $i ${CLIENTS[$i]} ${DOMAINS[$i]}

    client=${CLIENTS[$i]}
    domain=${DOMAINS[$i]}

    # generate private key
    openssl genrsa -out $client".KEY" 2048

    # create certificate signing request
    # set domain or IP in 'Common Name'
    openssl req -new -key $client."KEY" -out $client".CSR" 

    # sign with CA private key
    openssl x509 -req -in $client".CSR" -extfile $EXT_FILE -CA $CA".CRT" -CAkey $CA".KEY" -CAcreateserial -out $client."CRT" -days 5000

    # copy key and crt to one file
    cat $client".KEY" $client".CRT" > $client".PEM"
done

echo "DONE"

