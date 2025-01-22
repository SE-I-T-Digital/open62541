#!/bin/bash
# check if first parameter is --in-docker
if [ "$1" == "--in-docker" ]; then
    echo "Building in docker"
else
    # run this script in docker
    docker run -it --rm --user 1000:1000 -v $(pwd):$(pwd) -w $(pwd) wamr-open62541-build ./devtools/build.sh --in-docker
    exit 0
fi

# build open62541
export CMAKE_SDK_WASI=/opt/wasi-sdk/share/cmake/wasi-sdk-pthread.cmake
export CMAKE_SDK_WASI_THREAD=/opt/wasi-sdk/share/cmake/wasi-sdk-pthread.cmake
export CMAKE_SDK_WAMR=/opt/wamr-sdk/app-sdk/wamr_toolchain.cmake
export CMAKE_TOOLCHAIN_FILE=$CMAKE_SDK_WASI_THREAD

cmake -B build -DUA_ARCHITECTURE=wasm . && cmake --build build
