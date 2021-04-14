#!/bin/bash
#./start.sh $(cat s.txt)
for param in "$@"
do
./server --port $param --tnum 100 &
done