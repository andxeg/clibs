#!/bin/bash

# key names
CA="CA"
TERMINAL="TERMINAL"
SERVER="SERVER"

CA_CONFIG="ca.cnf"
TERMINAL_CONFIG="terminal.cnf"
SERVER_CONFIG="server.cnf"

TERMINAL_EXT="v3_terminal.ext"
SERVER_EXT="v3_server.ext"

CLIENTS=($TERMINAL $SERVER)
CONFIGS=($TERMINAL_CONFIG $SERVER_CONFIG)
EXTS=($TERMINAL_EXT $SERVER_EXT)

# generate key and crt for CA
openssl genrsa -out $CA".KEY" 2048
openssl req -x509 -new -sha1 -key $CA".KEY" -config $CA_CONFIG -days 10000 -out $CA".CRT"
openssl x509 -noout -text -in $CA".CRT" > $CA".txt"
echo >> $CA".CRT"
echo >> $CA".KEY" 


# generate key and crt for clients
for i in ${!CLIENTS[*]}
do
    printf "\n\nCLIENT NAME: %s\n\n" ${CLIENTS[$i]}

    client=${CLIENTS[$i]}
    config=${CONFIGS[$i]}
    extfile=${EXTS[$i]}

    # generate private key
    openssl genrsa -out $client".KEY" 1024

    # create certificate signing request
    # set domain or IP in 'Common Name'
    openssl req -new -key $client."KEY" -config $config -out $client".CSR" 

    # sign with CA private key
    openssl x509 -req -sha1 -in $client".CSR" -extfile $extfile -CA $CA".CRT" -CAkey $CA".KEY" -CAcreateserial -out $client."CRT" -days 5000

    # copy key and crt to one file
    cat $client".KEY" $client".CRT" > $client".PEM"

    # save crt to text file
    openssl x509 -noout -text -in $client".CRT" > $client".txt"

    # add empty string to crt, key and pem files
    echo >> $client".KEY"
    echo >> $client".CRT"
    echo >> $client".PEM"
done

echo "DONE"

