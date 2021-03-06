#!/bin/bash

OUTFILE=$(echo $$)

#valgrind ./testBW/testBW &> /tmp/lisp.$OUTFILE.out
#gdbserver 0.0.0.0:2345 ./testBW/testBW &> /tmp/bw.$OUTFILE.out
ulimit -c unlimited
FILES="~/.bashrc ~/.profile /etc/profile"

for FILE in "~/.bashrc" "~/.profile" "/etc/profile"
do
    if [ -e "$FILE" ]
    then
        . $FILE &>> /tmp/pcapReader.$OUTFILE.out
    fi
done

#cd testJBW
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:lib
echo "LIBPATH = " $LD_LIBRARY_PATH &>> /tmp/pcapReader.$OUTFILE.out


date &> /tmp/pcapReader.time
./pcapReaderBlk "$@" &>> /tmp/pcapReader.$OUTFILE.out
date &>> /tmp/pcapReader.time

#sleep 60
#rm -f /tmp/*c /tmp/*so /tmp/*out;
