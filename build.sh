#!/bin/bash

patt="*$1*"

if [[ -z ${patt} ]]; then
  patt="*"
fi

for cpp in $(ls ${patt}.cpp); do
  g++ -std=c++17 -O3 ${cpp} -o ${cpp/.cpp/}
done
