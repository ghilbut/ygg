# -*- coding: utf-8 -*-
import os

BASEPATH = os.path.dirname(__file__)
ROOTPATH = os.path.join(BASEPATH, '../../..')
ROOTPATH = os.path.abspath(ROOTPATH)



def get_thirdparty():

  class Path:
    def __init__(self, name, inc, lib):

      CPPPATH = []

      if isinstance(inc, str):
        inc = [inc]
      if not isinstance(inc, list):
        print "library(%s)'s include path should be a string or list."
        EXIT(-1)

      for p in inc:
        p = os.path.join(kBase, p)
        if not os.path.exists(p):
          print "library(%s)'s include path is not exists. - %s" % (name, p)
          EXIT(-1)
        CPPPATH.append(p)

      if not isinstance(lib, str):
        print "library(%s)'s binary path should be a string." % name
        EXIT(-1)

      lib = os.path.join(kBase, lib)
      if not os.path.exists(lib):
        print "library(%s)'s binary path is not exists. - %s" % (name, lib)
        EXIT(-1)

      self.CPPPATH = CPPPATH
      self.LIBPATH = lib


  kBase = os.path.join(ROOTPATH, '../ygg_thirdparty')
  kBase = os.path.abspath(kBase) 

  kLibs = {
    'gmock': [
        [ 'third_party/gmock-1.7.0/gtest/include', 'third_party/gmock-1.7.0/include' ],
        'gmock'
    ],
    'boost': [
        'third_party/boost_1_57_0/include',
        'third_party/boost_1_57_0/lib'
      ],
    'jsoncpp': [
        'third_party/jsoncpp-1.4.4/include',
        'jsoncpp'
      ]
  }

  names = []
  targets = []
  for name, params in kLibs.items():
    inc = params[0]
    lib = params[1]
    names.append(name)
    targets.append(Path(name, inc, lib))

  from collections import namedtuple
  targets = namedtuple('ThirdParty', names)._make(targets)
 
  return targets
