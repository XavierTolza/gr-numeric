#!/bin/sh
killall gdbserver
. ./set_env.sh
(gdbserver 0.0.0.0:2345 python python/$1.py --remote-debug;killall python;killall gdb;killall gdbserver)&
