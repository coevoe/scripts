#!/bin/sh

disk=$(df -h | awk 'NR==4{print $4, $5}')
printf "SSD %s\n" "$disk"
