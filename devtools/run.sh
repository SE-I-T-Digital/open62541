#!/bin/bash
# check if first parameter is --in-docker
if [ "$1" == "--in-docker" ]; then
    echo "Building in docker"
else
    # run this script in docker
    docker run -it --rm --user 1000:1000 -v $(pwd):$(pwd) -w $(pwd) wamr-open62541-build ./devtools/run.sh --in-docker
    exit 0
fi

iwasm build/bin/wasm_opcua.wasm
