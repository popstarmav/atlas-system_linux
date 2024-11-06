#!/bin/sh
trap 'echo Nope' SIGABRT SIGTERM SIGIO
while true; do sleep 1; done
