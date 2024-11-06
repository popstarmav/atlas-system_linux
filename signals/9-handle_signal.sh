#!/bin/sh
trap 'echo Nope' SIGABRT SIGTERM SIGIO
