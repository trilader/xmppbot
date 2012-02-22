#!/bin/bash

####EDIT####
MUC_SERVER='conference.shin-project.org'
MUC_ROOM='rwth'
MUC_NAME='Bot'

SERVER_USER='svnbot'
SERVER_PASSWORD='5h1n_svn_b0t'
SERVER_ADDRESS='shin-project.org'
SERVER_RESOURCE='TestBot'

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
