#!/bin/bash
sudo taskset 0x01 chrt -f 99 ./cache-benchmark
