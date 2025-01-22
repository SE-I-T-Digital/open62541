cmake_minimum_required(VERSION 3.1...3.12)

project(wasm-opcua)

add_compile_definitions(_WASI_EMULATED_SIGNAL __linux__)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(BUILD_SHARED_LIBS OFF)

set(CMAKE_EXECUTABLE_SUFFIX ".wasm")

include(/wasm-micro-runtime/core/iwasm/libraries/lib-socket/lib_socket_wasi.cmake)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/../include
    /wasm-micro-runtime/core/iwasm/libraries/lib-socket/inc
)


add_executable(wasm_opcua client.c)

target_include_directories(wasm_opcua PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../include /wasm-micro-runtime/core/iwasm/libraries/lib-socket/inc)
target_link_libraries(wasm_opcua open62541)
target_link_libraries(wasm_opcua -static open62541)
