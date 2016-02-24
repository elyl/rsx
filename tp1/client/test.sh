#!/bin/bash

for i in `seq 1 40000`;
do
    echo $i
    eval "./a.out 127.0.0.1 12345"
done   
