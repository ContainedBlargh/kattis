#!/bin/bash
python3 ../hdc/monolithize.py flow.h flow_ss.c; gcc -ggdb -pedantic -Wall -Wextra -rdynamic  flow_ss.c -o flow

