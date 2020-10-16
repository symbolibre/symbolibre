#! /usr/bin/env sh

msg=$1
shift

"$@"
read -p "$msg"
