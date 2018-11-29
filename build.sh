git submodule init
git submodule update

pushd toolchain/buildroot

make defconfig make defconfig BR2_DEFCONFIG=../buildroot.config.rs97
make

popd

source env.sh
pushd ports/sdl_test
make
file sdl_test
popd
