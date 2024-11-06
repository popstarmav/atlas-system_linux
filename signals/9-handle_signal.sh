#!/bin/sh
trap 'echo Nope' SIGABRT SIGIO SIGTERM
while :; do sleep 1; done

