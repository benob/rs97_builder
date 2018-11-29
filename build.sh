#!/bin/bash

git submodule init
git submodule update

ROOT=$PWD

pushd toolchain/buildroot

make defconfig make defconfig BR2_DEFCONFIG=$ROOT/buildroot.config.rs97
make

popd

pushd ports/sdl_test
source $ROOT/env.sh
make
file sdl_test
popd
