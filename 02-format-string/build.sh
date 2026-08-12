#!/bin/sh
# -no-pie: fixed addresses, so we can target `authenticated` directly.
# Format string bugs don't need a disabled stack canary or execstack --
# the whole point is that printf() itself does the writing for us.
set -e
gcc -no-pie -Wno-format-security -o vulnerable vulnerable.c
echo "Built ./vulnerable"
