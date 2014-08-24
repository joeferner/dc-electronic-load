#!/bin/bash -e

gcc -o build/test test.c stringUtils.c
build/test

