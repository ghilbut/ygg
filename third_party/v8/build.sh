#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"



export DEPOT_TOOLS_PATH="$DIR/depot_tools"
if [ ! -d "$DEPOT_TOOLS_PATH" ]
then
  git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
else
  pushd depot_tools
  git fetch origin
  git pull
  popd
fi

export PATH="$DEPOT_TOOLS_PATH:$PATH"
gclient


export V8_PATH="$DIR/v8"
if [ ! -d "$V8_PATH" ]
then
  fetch v8
fi

pushd $V8_PATH

export TARGET="6.5.107"
export BRANCH="$(git rev-parse --abbrev-ref HEAD)"
if [ "$BRANCH" != "heads/$TARGET" ]
then
  git fetch --all --tags --prune
  git checkout tags/$TARGET -b $TARGET
fi

gclient sync

tools/dev/v8gen.py -vv x64.release
tools/dev/v8gen.py -b "V8 Linux64 - builder" -m client.v8 x64.release
gn gen out.gn/x64.release \
       --args='is_component_build=false
               is_debug=false
               is_desktop_linux=false
               target_cpu="x64"
               v8_static_library=true'

ninja -C out.gn/x64.release
tools/run-tests.py --outdir out.gn/x64.release
popd
