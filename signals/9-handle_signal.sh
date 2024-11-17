#!/bin/sh
trap "echo Nope" ABRT SIGIO TERM; kill -ABRT $$; kill -SIGIO $$; kill -TERM $$
