# -*- coding:utf-8 -*-
#!/usr/bin/python

import os
import re
import zipfile
import tarfile
import threading
import urllib2
from subprocess import Popen, PIPE, STDOUT

rootpath = os.path.abspath(os.path.dirname(__file__))



class ArchiveReadyThread(threading.Thread):

  def __init__(self, num, params):
    threading.Thread.__init__(self)
    self.num = num
    self.basepath = os.path.join(rootpath, 'tmp')
    if not os.path.exists(self.basepath):
      os.makedirs(self.basepath)
    self.url     = params['url']
    self.success = params['success']
    self.failed  = params['failed']

  def download(self):
    num = self.num
    url = self.url
    basepath = self.basepath

    print '%d> download: %s' % (num, url)
    name = os.path.split(url)[1]
    path = os.path.join(basepath, name)
    response = urllib2.urlopen(url)
    data = response.read()
    tgz = open(path, "wb")
    tgz.write(data)
    tgz.close()
    return path

  def extract(self, filepath):
    num = self.num

    p = re.compile(r'^(?P<dir>.*)(/|\\)(?P<name>[^/\\]+).(?P<ext>zip|tgz|tar.gz)$')
    m = p.search(filepath)
    dir  = m.group('dir')
    name = m.group('name')
    ext  = m.group('ext')

    print '%d> extract: %s' % (num, filepath)
    if ext == 'zip':
      with zipfile.ZipFile(filepath, 'r') as zip:
        zip.extractall(os.path.dirname(dir))
    else:
      tar = tarfile.open(filepath, 'r:gz')
      # tar.extractall(self.basepath)
      tar.extractall(os.path.dirname(dir))
      tar.close()

    # archpath = os.path.join(os.path.split(filepath)[0], name)
    archpath = os.path.join(os.path.dirname(dir), name)
    return archpath
	

  def run(self):
    filepath = self.download()
    archpath = self.extract(os.path.join(filepath))
    self.success(self.num, archpath)



	

gmock = {
  'url': 'https://googlemock.googlecode.com/files/gmock-1.7.0.zip',
  'success': (lambda num, path: None),
  'failed':  (lambda num, path: failed(num, path))
}

jsoncpp = {
  'url': 'https://github.com/open-source-parsers/jsoncpp/archive/1.0.0.tar.gz',
  'success': (lambda num, path: None),
  'failed':  (lambda num, path: failed(num, path))
}

mongoose = {
  'url': 'https://github.com/cesanta/mongoose/archive/5.5.tar.gz',
  'success': (lambda num, path: None),
  'failed':  (lambda num, path: failed(num, path))
}

v8 = {
  'url': 'https://github.com/v8/v8-git-mirror/archive/3.31.47.tar.gz',
  'success': (lambda num, path: None),
  'failed':  (lambda num, path: failed(num, path))
}

cpp_netlib = {
  'url': 'http://storage.googleapis.com/cpp-netlib-downloads/0.11.1/cpp-netlib-0.11.1-final.zip',
  'success': (lambda num, path: None),
  'failed':  (lambda num, path: failed(num, path))
}

threads = [
  ArchiveReadyThread(1, gmock),
  ArchiveReadyThread(2, jsoncpp),
  ArchiveReadyThread(3, mongoose),
  ArchiveReadyThread(4, v8),
  ArchiveReadyThread(5, cpp_netlib)
]

for thread in threads:
  thread.start()
for thread in threads:
  thread.join()

import shutil
shutil.rmtree(os.path.abspath(os.path.join(rootpath, 'tmp')))