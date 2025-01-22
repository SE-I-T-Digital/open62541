FROM ubuntu:24.04 

ARG DEBIAN_FRONTEND=noninteractive
ENV TZ=UTC

RUN apt-get update && \
    apt-get install --no-install-recommends -y software-properties-common \
    git \
    gpg \
    python3 \
    python3-pip \
    python3-dev \
    libssl-dev \
    make \
    pkg-config \
    flex \
    bison \
    iasl \
    nasm \
    build-essential \
    uuid-dev \
    texinfo \
    subversion \
    ninja-build \
    socat \
    gawk \
    tmux \
    nano \
    libglib2.0-dev  \
    zlib1g-dev \
    libpixman-1-dev \
    libgmp3-dev \
    libmpfr-dev \
    libmount-dev \
    libtasn1-dev \
    apt-transport-https \
    apt-utils \
    ca-certificates \
    ccache \
    cmake \
    curl \
    g++-multilib \
    gnupg \
    libgcc-12-dev \
    lib32gcc-12-dev \
    lsb-release \
    ocaml \
    ocamlbuild \
    tree \
    tzdata \
    unzip \
    valgrind \
    vim \
    wget \
    zip \
    python3-sphinx \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

    WORKDIR /apps/qemu
RUN git clone -b "v9.1.2" --depth=1 https://git.qemu.org/git/qemu.git .

WORKDIR /apps/ovfm
RUN git clone -b "edk2-stable202411" --recursive https://github.com/tianocore/edk2.git .

WORKDIR /apps/qemu

RUN ./configure --with-pkgversion=lncm-v9.1.2-0.1 \
    --target-list=x86_64-softmmu,arm-softmmu,aarch64-softmmu \
    --audio-drv-list= \
    --disable-gio \
    --enable-kvm --enable-attr --enable-linux-user --enable-tcg \
    --static

RUN make
RUN make install

WORKDIR /apps/ovfm

RUN mkdir -p Conf
RUN echo ACTIVE_PLATFORM=OvmfPkgx64.dsc > Conf/target.txt
RUN echo TARGET_ARCH=x64 >> Conf/target.txt
RUN echo TOOL_CHAIN_TAG=GCC5 >> Conf/target.txt
RUN echo BUILD_RULE_CONF=Conf/build_rule.txt >> Conf/target.txt
RUN echo TOOL_CHAIN_CONF=Conf/tools_def.txt >> Conf/target.txt

RUN OvmfPkg/build.sh -a X64 -b RELEASE

RUN mkdir -p /apps/ovmf /apps/qemu 

WORKDIR /zig-lang/

# USER ubuntu
# ## Install rust
# ENV CARGO_HOME=/home/ubuntu/.cargo
# ENV RUSTUP_HOME=/home/ubuntu/.local/share/rustup

# RUN curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
# ENV PATH=/home/ubuntu/.cargo/bin:/home/ubuntu/.local/share/rustup/bin:$PATH
# RUN cargo install cargo-wasi cargo-wasix wasm-opt 

# # build a simple rust application in order to download the toolchain
# WORKDIR /home/ubuntu/app/examples
# RUN cargo new wasix-hello-world
# WORKDIR /home/ubuntu/app/examples/wasix-hello-world
# RUN cargo wasix build --release
# RUN cargo wasi build --release
# RUN cargo build --release
# RUN rm -rf /home/ubuntu/app/examples

# USER root

#RUN apt-get update && \
#    apt-get install --no-install-recommends -y gzip

WORKDIR /tmp/go 
RUN curl -L -O https://go.dev/dl/go1.23.3.linux-amd64.tar.gz
RUN tar -C /usr/local -xzf go1.23.3.linux-amd64.tar.gz 
RUN rm -rf /tmp/go

ENV PATH=$PATH:/usr/local/go/bin

USER root

#
# install emsdk
WORKDIR /opt
RUN git clone https://github.com/emscripten-core/emsdk.git

ARG EMSDK_VER=3.1.43
WORKDIR /opt/emsdk
RUN  git pull \
  && ./emsdk install ${EMSDK_VER} \
  && ./emsdk activate ${EMSDK_VER} \
  && echo "source /opt/emsdk/emsdk_env.sh" >> /root/.bashrc

#
# install wasi-sdk
ARG WASI_SDK_VER=22
RUN wget -c --progress=dot:giga https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASI_SDK_VER}/wasi-sdk-${WASI_SDK_VER}.0-linux.tar.gz -P /opt \
  && tar xf /opt/wasi-sdk-${WASI_SDK_VER}.0-linux.tar.gz -C /opt \
  && ln -sf /opt/wasi-sdk-${WASI_SDK_VER}.0 /opt/wasi-sdk \
  && rm /opt/wasi-sdk-${WASI_SDK_VER}.0-linux.tar.gz

#
#install wabt
ARG WABT_VER=1.0.33
RUN wget -c --progress=dot:giga https://github.com/WebAssembly/wabt/releases/download/${WABT_VER}/wabt-${WABT_VER}-ubuntu.tar.gz -P /opt \
  && tar xf /opt/wabt-${WABT_VER}-ubuntu.tar.gz -C /opt \
  && ln -sf /opt/wabt-${WABT_VER} /opt/wabt \
  && rm /opt/wabt-${WABT_VER}-ubuntu.tar.gz

#
# install bazelisk
ARG BAZELISK_VER=1.17.0
RUN mkdir /opt/bazelisk \
  && wget -c --progress=dot:giga https://github.com/bazelbuild/bazelisk/releases/download/v${BAZELISK_VER}/bazelisk-linux-amd64 -P /opt/bazelisk \
  && chmod a+x /opt/bazelisk/bazelisk-linux-amd64 \
  && ln -fs /opt/bazelisk/bazelisk-linux-amd64 /opt/bazelisk/bazel

#
# install clang+llvm
ARG LLVM_VER=16
RUN apt-get purge -y clang-14 || exit 0
RUN apt-get purge -y llvm-14 || exit 0 
RUN apt-get autoremove -y

WORKDIR /etc/apt/apt.conf.d
RUN touch 99verfiy-peer.conf \
  && echo "Acquire { https::Verify-Peer false }" > 99verfiy-peer.conf

WORKDIR /tmp
#RUN wget --progress=dot:giga https://apt.llvm.org/llvm.sh \
#  && chmod a+x ./llvm.sh \
#  && ./llvm.sh ${LLVM_VER} all

# Workaround due to https://github.com/llvm/llvm-project/issues/62475
# hadolint ignore=DL3008
RUN set -ex \
    && echo "deb http://apt.llvm.org/bookworm/ llvm-toolchain-bookworm-${LLVM_VER} main" > /etc/apt/sources.list.d/apt.llvm.org.list \
    && wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key | tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc \
    && apt-get update \
    && apt-get install -y \
    clang-${LLVM_VER} lldb-${LLVM_VER} lld-${LLVM_VER} clangd-${LLVM_VER} clang-tidy-${LLVM_VER} clang-format-${LLVM_VER} clang-tools-${LLVM_VER} \
    llvm-${LLVM_VER}-dev lld-${LLVM_VER} lldb-${LLVM_VER} llvm-${LLVM_VER}-tools libomp-${LLVM_VER}-dev libc++-${LLVM_VER}-dev libc++abi-${LLVM_VER}-dev \
    libclang-common-${LLVM_VER}-dev libclang-${LLVM_VER}-dev libclang-cpp${LLVM_VER}-dev libunwind-${LLVM_VER}-dev \
    libclang-rt-${LLVM_VER}-dev libpolly-${LLVM_VER}-dev --no-install-recommends \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

#
# [Optional]

#
# Install pip
# hadolint ignore=DL3008
RUN apt-get update \
  && apt-get install -y --reinstall python3-venv python3-pip --no-install-recommends \
  && apt-get clean -y \
  && rm -rf /var/lib/apt/lists/*

#
# Install required python packages
# hadolint ignore=DL3013
RUN python3 -m pip install --no-cache-dir --break-system-packages black nose pycparser pylint

#
# Install github-cli. It doens't work as a feature of devcontainer.json
ARG GH_CLI_VER=2.32.0
WORKDIR /tmp
RUN wget -q https://github.com/cli/cli/releases/download/v${GH_CLI_VER}/gh_${GH_CLI_VER}_linux_amd64.deb \
  && dpkg -i gh_${GH_CLI_VER}_linux_amd64.deb

#
# Install NodeJS
RUN wget -qO- https://deb.nodesource.com/setup_20.x | bash -
# hadolint ignore=DL3008
RUN apt-get install -y nodejs --no-install-recommends

# set path
ENV PATH="/opt/bazelisk:/usr/lib/llvm-${LLVM_VER}/bin:${PATH}"
ENV CC=/usr/lib/llvm-${LLVM_VER}/bin/clang CXX=/usr/lib/llvm-${LLVM_VER}/bin/clang++
RUN printf "%s\n" "PS1='\n[ \u@wamr-dev-docker \W ]\n$ '" >> /root/.bashrc \
  && apt-get autoremove -y \
  && apt-get clean -y \
  && rm -rf /var/lib/apt/lists/* \
  && rm -rf /tmp/*

WORKDIR /wasm-micro-runtime
RUN git clone -b WAMR-2.2.0 https://github.com/bytecodealliance/wasm-micro-runtime.git .

RUN cmake -B ./build -DWAMR_BUILD_INTERP=1 -DWAMR_BUILD_LIB_PTHREAD=1 -DWAMR_BUILD_LIB_WASI_THREADS=1  product-mini/platforms/linux && cmake --build ./build --target install

WORKDIR /opt/wamr-core
RUN cp -r /wasm-micro-runtime/core/iwasm/libraries /opt/wamr-core

WORKDIR /wamr-sdk
RUN git clone -b main https://github.com/bytecodealliance/wamr-app-framework.git . && git checkout e97860611e371d91d2ef25101b02b29719476a48
RUN cd ./deps && LVGL=0 ./download.sh
RUN cd ./wamr-sdk  && ./build_sdk.sh

USER root
RUN mv /wamr-sdk/wamr-sdk/out/default /opt/wamr-sdk
RUN mkdir -p /opt/wamr-sdk/app-sdk/libc-builtin-sysroot/share
RUN cp /wamr-sdk/wamr-sdk/app/libc-builtin-sysroot/share/defined-symbols.txt /opt/wamr-sdk/app-sdk/libc-builtin-sysroot/share/
RUN cp /wasm-micro-runtime/core/config.h /opt/
RUN cp /wasm-micro-runtime/core/shared/platform/linux/platform_internal.h /opt/wamr-sdk/runtime-sdk/include


RUN cp /wamr-sdk/wamr-sdk/app/wasi_toolchain.cmake /opt/wamr-sdk/app-sdk/wasi_toolchain.cmake
RUN echo "set(WASI_SDK_DIR /opt/wasi-sdk/)" > /opt/wamr-sdk/app-sdk/wamr_toolchain.patched.cmake
RUN cat /opt/wamr-sdk/app-sdk/wamr_toolchain.cmake >> /opt/wamr-sdk/app-sdk/wamr_toolchain.patched.cmake
RUN mv /opt/wamr-sdk/app-sdk/wamr_toolchain.patched.cmake /opt/wamr-sdk/app-sdk/wamr_toolchain.cmake

RUN rm -rf /wamr-sdk

WORKDIR /tmp/tinygo

RUN curl -L -O https://github.com/tinygo-org/tinygo/releases/download/v0.34.0/tinygo_0.34.0_amd64.deb
RUN dpkg -i tinygo_0.34.0_amd64.deb
RUN rm -rf /tmp/tinygo

WORKDIR /workspaces

USER ubuntu
