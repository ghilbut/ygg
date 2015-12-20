#!/bin/bash
#set -x # echo on



# if there is no wget, install wget.
if ! which wget >/dev/null; then 
  tar -vxzf wget-1.17.1.tar.gz
  pushd wget-1.17.1
  ./configure --with-ssl=openssl --with-libssl-prefix=/usr/local/opt/openssl
  make
  sudo make install
  popd
  rm -rf wget-1.17.1
fi



kTmp="$(pwd)/tmp"
if [ ! -d "$kTmp" ]; then
  mkdir "$kTmp"
fi



# boost 1.60.0
if [ ! -d "$kTmp/boost_1_60_0" ]; then
  wget 'http://jaist.dl.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.gz'
  tar -vxzf "boost_1_60_0.tar.gz" -C "$kTmp"
  rm -rf "boost_1_60_0.tar.gz"
fi

# bzip2 1.0.6
if [ ! -d "$kTmp/bzip2-1.0.6" ]; then
  wget 'http://www.bzip.org/1.0.6/bzip2-1.0.6.tar.gz'
  tar -vxzf "bzip2-1.0.6.tar.gz" -C "$kTmp"
  rm -rf "bzip2-1.0.6.tar.gz"
  export BZIP2_SOURCE="$kTmp/bzip2-1.0.6"
fi

# icu 56.1
if [ ! -d "$kTmp/icu" ]; then
  wget 'http://download.icu-project.org/files/icu4c/56.1/icu4c-56_1-src.tgz'
  tar -vxzf "icu4c-56_1-src.tgz" -C "$kTmp"
  rm -rf "icu4c-56_1-src.tgz"
fi

# python 2.7.11
if [ ! -d "$kTmp/Python-2.7.11" ]; then
  wget 'http://python.org/ftp/python/2.7.10/Python-2.7.11.tgz'
  tar -vxzf "Python-2.7.11.tgz" -C "$kTmp"
  rm -rf "Python-2.7.11.tgz"
  export PYTHON_ROOT="$kTmp/Python-2.7.11"
  export PYTHON_VERSION=2.7
fi

# zlib 1.2.8
if [ ! -d "$kTmp/zlib-1.2.8" ]; then
  tar -vxzf "zlib-1.2.8.tar.gz" -C "$kTmp"
  rm -rf "zlib-1.2.8.tar.gz"
  export ZLIB_SOURCE="$kTmp/zlib-1.2.8"
fi



# icu
pushd "$kTmp/icu/source"

# see "https://github.com/aosm/ICU/blob/master/icuSources/runConfigureICU"
if [ "$(uname)" == "Darwin" ]; then
# Do something under Mac OS X platform        
kPlatform="MacOSX"
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
# Do something under Linux platform
kPlatform="Linux"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
# Do something under Windows NT platform
kPlatform="MinGW"
fi

#chmod +x "configure" "install-sh" "runConfigureICU"
#./runConfigureICU "$kPlatform" --prefix="$kTmp" --enable-static
#make
#make install
popd



# compile
pushd "$kTmp/boost_1_60_0"
chmod +x bootstrap.sh
./bootstrap.sh
./b2 --prefix="$kTmp/../.." --without-mpi toolset=clang cxxflags="-std=c++11 -stdlib=libc++" linkflags="-stdlib=libc++" variant=debug,release threading=multi link=static runtime-link=static address-model=64 --layout=tagged install
popd



rm -rf "$kTmp"
