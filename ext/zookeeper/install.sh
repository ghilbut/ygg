#!/bin/bash

install_path=$(pwd)/..
echo $install_path
zookeeper='zookeeper-3.4.8'
echo $zookeeper

if [ ! -d $zookeeper ]; then
  wget http://apache.mirror.cdnetworks.com/zookeeper/$zookeeper/$zookeeper.tar.gz
  tar -vxzf $zookeeper.tar.gz
  pushd $zookeeper/src/c
  ./configure --prefix=$install_path --disable-shared
  make install
  popd
fi

