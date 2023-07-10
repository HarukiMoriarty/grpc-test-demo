#! /bin/bash
# Author: Zhenghong Yu
# zyu379@wisc.edu

echo "####################### Step 1: Upgrade apt #######################"
echo "Do you want to upgrade apt? [Y/n]"
read userInput
if [[ "$userInput" == "Y" ]]
then
    sudo apt update
    sudo apt upgrade
    echo "####################### Step 1: Finished #######################"
else
    echo "####################### Step 1: Passed #######################"
fi


echo "####################### Step 2: Install pkgs #######################"
echo "Do you want to install zip, make, g++, libjemalloc-dev, libboost-dev? [Y/n]"
read userInput
if [[ "$userInput" == "Y" ]]
then
    sudo apt install -y zip make g++ libjemalloc-dev libboost-dev
    echo "####################### Step 2: Finished #######################"
else
    echo "####################### Step 2: Passed #######################"
fi


echo "####################### Step 3: Install cmake #######################"
echo "Do you want to install cmake? [Y/n]"
read userInput
if [[ "$userInput" == "Y" ]]
then
    cd ~
    wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1.tar.gz
    tar -xf cmake-3.25.1.tar.gz
    cd cmake-3.25.1
    ./bootstrap
    make -j 32
    sudo make install
    export PATH=$PATH:/usr/local/bin
    echo "####################### Step 3: Finished #######################"
else
    echo "####################### Step 3: Passed #######################"
fi


echo "####################### Step 4: Install glog #######################"
echo "Do you want to install glog? [Y/n]"
read userInput
if [[ "$userInput" == "Y" ]]
then
    cd ~
    git clone https://github.com/google/glog.git
    cd glog
    cmake -S . -B build -G "Unix Makefiles"
    cmake --build build -j 32
    sudo cmake --build build --target install
    echo "####################### Step 4: Finished #######################"
else
    echo "####################### Step 4: Passed #######################"
fi


echo "####################### Step 5: Install gflags #######################"
echo "Do you want to install gflags? [Y/n]"
read userInput
if [[ "$userInput" == "Y" ]]
then
    sudo apt-get install libgflags-dev
    echo "####################### Step 5: Finished #######################"
else
    echo "####################### Step 5: Passed #######################"
fi


echo "####################### Step 6: Install protoc #######################"
echo "Do you want to install protoc? [Y/n]"
read userInput
if [[ "$userInput" == "Y" ]]
then
    sudo apt install -y protobuf-compiler
    echo "####################### Step 6: Finished #######################"
else
    echo "####################### Step 6: Passed #######################"
fi


echo "####################### Step 7: Install grpc #######################"
echo "Do you want to install grpc? [Y/n]"
read userInput
if [[ "$userInput" == "Y" ]]
then
    cd ~
    mkdir -p ~/local_lib
    setenv PATH "~/local_lib:$PATH"
    git clone --recurse-submodules -b v1.50.0 --depth 1 --shallow-submodules https://github.com/grpc/grpc
    cd grpc
    mkdir -p cmake/build
    pushd cmake/build
    cmake -DgRPC_INSTALL=ON \
    -DgRPC_BUILD_TESTS=OFF \
    -DCMAKE_INSTALL_PREFIX="~/local_lib" \
    ../..
    make -j 32
    make install
    echo "export PATH=$PATH:/users/zhhong/local_lib" >> ~/.bashrc
    source ~/.bashrc
    echo "####################### Step 7: Finished #######################"
else
    echo "####################### Step 7: Passed #######################"
fi