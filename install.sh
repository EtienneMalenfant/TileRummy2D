#!/bin/bash
set -e

cd libs

echo "Extracting Boost..."
if [ ! -d boost ]; then
    tar -xzf boost.tar.gz -C .
else
    echo "Boost already extracted."
fi

echo "Extracting SFML..."
if [ ! -d sfml ]; then
    tar -xzf sfml.tar.gz -C .
else
    echo "SFML already extracted."
fi
cd ..

echo "Building TileRummy2D..."
rm -r build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
cd ..

echo "Copying data files..."
mkdir -p bin
mkdir -p bin/data
cp -ru data/* bin/data
cp build/TileRummy2D bin

echo "Done. The game is in the bin directory."
