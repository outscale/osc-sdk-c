#!/bin/sh

curl -s https://api.github.com/repos/outscale/osc-api/tags | grep  $(git grep OSC_API_VERSION osc_sdk.h | cut -d '"' -f 2) | grep name | cut -d '"' -f 4 | tail -n 1
