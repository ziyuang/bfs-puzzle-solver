#!/bin/bash

for cpp in $(ls *.cpp)
do
  g++ -std=c++17 -O3 $cpp -o ${cpp/.cpp/}
done