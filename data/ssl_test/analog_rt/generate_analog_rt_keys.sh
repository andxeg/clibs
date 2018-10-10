#!/bin/bash

# key names
CA="CA"
TERMINAL="TERMINAL"
SERVER="SERVER"

CLIENTS=($TERMINAL $SERVER)

EXT_FILE="v3.ext"

# generate key and crt for CA
openssl genrsa -out $CA".KEY" 2048
printf "\n\nCREATE SELF-SIGNED CRT FOR CA\n\n"
openssl req -x509 -new -key $CA".KEY" -days 10000 -out $CA".CRT"


# generate key and crt for clients
for i in ${!CLIENTS[*]}
do
    printf "\n\n%4d: CLIENT NAME: %s \n\n" $i ${CLIENTS[$i]}

    client=${CLIENTS[$i]}

    # generate private key
    openssl genrsa -out $client".KEY" 1024

    # create certificate signing request
    openssl req -new -key $client."KEY" -out $client".CSR" 

    # sign with CA private key
    openssl x509 -req -in $client".CSR" -extfile $EXT_FILE -CA $CA".CRT" -CAkey $CA".KEY" -CAcreateserial -out $client."CRT" -days 5000

    # copy key and crt to one file
    cat $client".KEY" $client".CRT" > $client".PEM"
done

echo "DONE"

