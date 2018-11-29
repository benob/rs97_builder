
SYSROOT=$PWD/toolchain/buildroot/output/host/mipsel-buildroot-linux-musl/sysroot/
BIN=$PWD/toolchain/buildroot/output/host/bin

export ARCH=mips
export CROSS_COMPILE=mipsel-linux-
export CC="${CROSS_COMPILE}gcc --sysroot=$SYSROOT"
export CXX="${CROSS_COMPILE}g++ --sysroot=$SYSROOT"
export NM=${CROSS_COMPILE}nm
export LD=${CROSS_COMPILE}ld
export RANLIB=${CROSS_COMPILE}ranlib
export AR=${CROSS_COMPILE}ar

export PATH=$BIN:$SYSROOT/usr/bin/:$PATH

export CFLAGS="-DRS97 -Ofast -march=mips32 -mtune=mips32 -fomit-frame-pointer"
export CXXFLAGS=$CFLAGS
