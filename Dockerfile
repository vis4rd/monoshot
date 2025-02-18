FROM ubuntu:25.04

RUN apt-get -y update

RUN apt-get -y install \
    ccache \
    clang \
    clang-format \
    clang-tidy \
    cmake \
    curl \
    g++ \
    git \
    jq \
    libgl-dev \
    libxcursor-dev \
    libxi-dev \
    libxinerama-dev \
    libxrandr-dev \
    python3

ENTRYPOINT [ "/bin/sh", "-c" ]
