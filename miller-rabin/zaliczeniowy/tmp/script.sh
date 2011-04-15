#!/bin/bash

while read line 
do
	./a.out -p "$line"
done
exit 0
