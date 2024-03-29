#!/bin/sh
set -eu

die () { echo "$1" ; exit 1 ; }

PORT=5151

if [ ! -x client ] ; then die "client not found" ; fi
if [ ! -x server ] ; then die "server not found" ; fi


(./server $PORT > server.txt) &
SPID=$!

trap 'kill $SPID ; rm server.txt' EXIT

sleep 2s

while read -r str shift ; do
    recv=$(./client 127.0.0.1 5151 "$str" "$shift")
    if [ "$str" != "$recv" ] ; then die "error $str $shift" ; fi
done < tests.txt

if ! diff server.txt results.txt > /dev/null ; then
    die "error: server output" ;
fi

echo "OK"
