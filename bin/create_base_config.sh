#!/bin/bash

####EDIT####
MUC_SERVER='conference.example.org'
MUC_ROOM='room'
MUC_NAME='XmmpBot'

SERVER_USER='username'
SERVER_PASSWORD='secret'
SERVER_ADDRESS='example.org'
SERVER_RESOURCE='Bot'

####EDIT END####


./xmppbot --edit muc.server --value $MUC_SERVER
./xmppbot --edit muc.room --value $MUC_ROOM
./xmppbot --edit muc.name --value $MUC_NAME

./xmppbot --edit server.user --value $SERVER_USER
./xmppbot --edit server.password --value $SERVER_PASSWORD
./xmppbot --edit server.address --value $SERVER_ADDRESS
./xmppbot --edit server.resource --value $SERVER_RESOURCE

./xmppbot --get server
./xmppbot --get muc
