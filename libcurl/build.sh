#!/bin/bash -xe

# Validate the curl checksum
CURL_VERSION="8.5.0"
CURL_SHA256_CHECKSUM="05fc17ff25b793a437a0906e0484b82172a9f4de02be5ed447e0cab8c3475add"
echo "$CURL_SHA256_CHECKSUM curl-$CURL_VERSION.tar.gz" | sha256sum --check --status

# Unpack curl
rm -rf "./curl-$CURL_VERSION/"
tar -xvf curl-$CURL_VERSION.tar.gz

# We want to install to the parent directory
OUTPUT_DIR=$(realpath ../output)
mkdir -p "$OUTPUT_DIR"

# Configure curl
cd "./curl-$CURL_VERSION"
./configure --with-openssl --enable-websockets --prefix=$OUTPUT_DIR

# Build and install curl
make
make install
