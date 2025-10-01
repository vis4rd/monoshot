FROM alpine:3.20

# Install build essentials and tools on Alpine
# Package index: https://pkgs.alpinelinux.org/packages
RUN apk add --no-cache \
    bash \
    ca-certificates \
    ccache \
    cmake \
    curl \
    dotnet6-runtime \
    git \
    jq \
    libx11-dev \
    libxcursor-dev \
    libxi-dev \
    libxinerama-dev \
    libxrandr-dev \
    make \
    mesa-dev \
    openssh

# Install clang19 and clang19-extra-tools from Alpine edge (packages are in edge/community)
# Keep the base image on stable 3.20 and only pull these specific packages from edge repos
RUN apk add --no-cache \
    --repository=https://dl-cdn.alpinelinux.org/alpine/edge/main \
    --repository=https://dl-cdn.alpinelinux.org/alpine/edge/community \
    clang19-extra-tools \
    g++

ENTRYPOINT [ "/bin/sh", "-c" ]
