#!/bin/bash

set -eE

if [ "$#" -eq 0 ]; then
    trap "echo [clone git FAIL]" ERR
    if [ ! -d "osc-ricochet-2" ]; then
	    git clone https://github.com/outscale/osc-ricochet-2
    fi

    trap "echo [make ricochet-2 FAIL]" ERR
    cd osc-ricochet-2

    cargo build
    cargo run -- ../ricocher-cfg.json > /dev/null  &

    sleep 5

    cd ..
fi

export OSC_ENDPOINT_API=127.0.0.1:3000
export OSC_ACCESS_KEY=11112211111110000000
export OSC_SECRET_KEY=0000001111112222223333334444445555555666

trap "echo [make integration-test FAIL]" ERR
make integration-test

trap "echo [pkill ricochet-2 FAIL]" ERR
pkill ricochet-2
