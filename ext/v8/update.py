#!/usr/bin/python
# -*- coding:utf-8 -*-

import os
import platform
import sys

BASEPATH = os.path.dirname(__file__)
BASEPATH = os.path.abspath(BASEPATH)

print BASEPATH

# ready depot_tools environment
DEPOT_TOOLS_PATH = os.path.join(BASEPATH, 'depot_tools')
if not os.path.exists(DEPOT_TOOLS_PATH):
  os.system('git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git')

if not os.path.isdir(DEPOT_TOOLS_PATH):
  print '%s is not a directory. process is stopped.' % DEPOT_TOOLS_PATH
  exit(-1)

os.chdir(DEPOT_TOOLS_PATH)
os.system('git pull')
os.chdir(BASEPATH)

os.environ["PATH"] += os.pathsep + DEPOT_TOOLS_PATH
os.system('gclient')

# ready v8 environment
V8_PATH = os.path.join(BASEPATH, 'v8')
if not os.path.exists(V8_PATH):
  os.system('fetch v8')
  os.chdir(V8_PATH)
  os.system('git config branch.autosetupmerge always')
  os.system('git config branch.autosetuprebase always')
  os.system('git new-branch work')
  os.chdir(BASEPATH)

if not os.path.isdir(V8_PATH):
  print '%s is not a directory. process is stopped.' % V8_PATH
  exit(-1)

# update
os.chdir(V8_PATH)
os.system('git pull')
os.system('gclient sync')

# build
'''
[sys.platform]
linux2 : Linux (2.x and 3.x)
win32  : Windows
cygwin : Windows/Cygwin
darwin : Mac OS X
os2    : OS/2
os2emx : OS/2 EMX
riscos : RiscOS
atheos : AtheOS
'''

PLATFORM = sys.platform
OPTIONS = { '32bit': 'ia32', '64bit': 'x64' }
ARCHITECTURE = platform.architecture()[0]
ARCHITECTURE = OPTIONS[ARCHITECTURE]

if PLATFORM == 'linux2':
  target = '%s.release' % ARCHITECTURE
  os.system('make %s' % target)
  #os.system('make native')
  os.system('out/%s/unittests' % target)
elif PLATFORM == 'darwin':
  os.system('build/gyp_v8 -Dtarget_arch=%s' % ARCHITECTURE)
  os.system('xcodebuild -project build/all.xcodeproj -configuration Release')
  os.system('xcodebuild/Release/unittests')
else:
  pass


