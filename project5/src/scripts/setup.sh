#!/bin/sh

if [ "$#" -ne 1 ]; then
  echo "Usage: $0 PROJECT_ROOT" >&2
  exit 1
fi
if ! [ -e "$1" ]; then
  echo "PROJECT_ROOT not found" >&2
  exit 1
fi
if ! [ -d "$1" ]; then
  echo "PROJECT_ROOT not a directory" >&2
  exit 1
fi

export PROJECT_ROOT=$1

source $PROJECT_ROOT/devel/setup.sh
