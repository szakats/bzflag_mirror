#!/bin/sh
# script to prepare bzflag sources
touch NEWS
aclocal
autoheader
automake --add-missing
autoconf

echo BZFlag sources are now prepared. To build here, run:
echo " ./configure --enable-robots"
echo " make"
