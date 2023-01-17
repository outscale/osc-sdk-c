#!/bin/sh

echo $COGNAC_CONFIG  | grep target-api > /dev/null
if [ $? -ne 0 ]; then
    echo --target-api=$(curl -s https://api.github.com/repos/outscale/osc-api/tags | grep  $(git grep OSC_API_VERSION osc_sdk.h | cut -d '"' -f 2) | grep name | cut -d '"' -f 4 | tail -n 1)
fi
