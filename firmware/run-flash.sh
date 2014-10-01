#!/bin/bash

st-flash --reset write build/*.bin 0x8000000
