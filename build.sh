#!/bin/bash

patt="*$1*"

for cpp in $(ls ${patt}.cpp); do
  g++ -std=c++17 -O3 ${cpp} -o ${cpp/.cpp/}
done
