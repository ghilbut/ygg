#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SRC="$DIR/src"
TMP="$DIR/tmp"

mkdir -p "$TMP"

set -x # echo on

# boost 1.66.0
export BOOST_SOURCE="$TMP/boost_1_66_0"
if [ ! -d "$BOOST_SOURCE" ]; then
  tar -xzf "$SRC/boost_1_66_0.tar.gz" -C "$TMP"
fi

# bzip2 1.0.6
export BZIP2_SOURCE="$TMP/bzip2-1.0.6"
if [ ! -d "$BZIP2_SOURCE" ]; then
  tar -xzf "$SRC/bzip2-1.0.6.tar.gz" -C "$TMP"
fi

# icu 59.1
export ICU_PATH="$TMP/icu"
if [ ! -d "$ICU_PATH" ]; then
  tar -xzf "$SRC/icu4c-59_1-src.tgz" -C "$TMP"
fi

# python 3.6.4
export PYTHON=`which python3`
export PYTHON_ROOT="$TMP/Python-3.6.4"
if [ ! -d "$PYTHON_ROOT" ]; then
  tar -xzf "$SRC/Python-3.6.4.tgz" -C "$TMP"
fi
export PYTHON_VERSION=3.6

# zlib 1.2.11
export ZLIB_SOURCE="$TMP/zlib-1.2.11"
if [ ! -d "$ZLIB_SOURCE" ]; then
  tar -xzf "$SRC/zlib-1.2.11.tar.gz" -C "$TMP"
fi


# icu
pushd "$TMP/icu/source"

# see "https://github.com/aosm/ICU/blob/master/icuSources/runConfigureICU"
if [ "$(uname)" == "Darwin" ];
then
  # Do something under Mac OS X platform        
  kPlatform="MacOSX"
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ];
then
  # Do something under Linux platform
  kPlatform="Linux"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ];
then
  # Do something under Windows NT platform
  kPlatform="MinGW"
fi

#chmod +x "configure" "install-sh" "runConfigureICU"
#./runConfigureICU "$kPlatform" --prefix="$TMP" --enable-static
#make
#make install
popd



# compile
pushd "$TMP/boost_1_66_0"
./bootstrap.sh
./b2 address-model=64 \
     cxxflags="-std=c++14" \
     link=static \
     runtime-link=static \
     threading=multi \
     variant=debug,release \
     --layout=tagged \
     --prefix="$DIR/.." \
     --without-coroutine \
     --without-mpi \
     install
popd
