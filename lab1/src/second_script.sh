#!/bin/bash
echo "There were $# parameters passed"
s=0
for param in "$@"
do
s=$(( $s + $param ))
done
echo "All parameters add up to $s"
echo "Averege is $(( $s / $# ))"