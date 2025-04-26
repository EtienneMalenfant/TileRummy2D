
rm -r build-windows
mkdir build-windows
cd build-windows
cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw-w64-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
