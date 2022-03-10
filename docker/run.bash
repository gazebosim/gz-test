#!/usr/bin/env bash

export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

ign-test -v 4 "$@"
