#!/usr/bin/env python
import BZFlag.ListServer
for server in BZFlag.ListServer.getDefault().filteredList():
    print server.info()

