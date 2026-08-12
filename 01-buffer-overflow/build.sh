#!/bin/sh
# Compiles the lab with protections deliberately disabled so the classic
# stack-smashing technique works the way it does in the textbooks:
#   -fno-stack-protector : no stack canary
#   -no-pie               : fixed, predictable addresses (no ASLR of the binary itself)
#   -z execstack          : stack is executable (not required for this lab, kept for clarity)
# 64-bit build: a saved return address is 8 bytes.
set -e
gcc -fno-stack-protector -no-pie -z execstack -o vulnerable vulnerable.c
echo "Built ./vulnerable"
