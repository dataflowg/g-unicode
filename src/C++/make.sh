#!/bin/bash
OUTPUT_PATH=../LabVIEW/G-Unicode/lib

g++ -shared -fPIC -o g_unicode_64.so *.cpp -O3
mkdir -p $OUTPUT_PATH
mv g_unicode_64.so $OUTPUT_PATH
