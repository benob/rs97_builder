git submodule init
git submodule update

cd buildroot

make defconfig make defconfig BR2_DEFCONFIG=../buildroot.config.rs97
make

cd ..
