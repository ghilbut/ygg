#-*- coding:utf-8 -*-

import os
import re
import tarfile
import urllib
import urllib2
import zipfile

from subprocess import call



################################################################
##
################################################################

def download(url, filename):

  print '#### download [%s] from [%s]' % (filename, url)

  np = urllib.urlopen(url)
  try:
    np.getheader('Content-Length')
  except:
    pass
  np.close()

  np = urllib2.urlopen(url)
  meta = np.info()

  filesize = int(meta.getheaders("Content-Length")[0])
  written_size = 0
  buffer_size = 4096

  fp = open(filename, 'wb')
  while True:
    buffer = np.read(buffer_size)
    if not buffer:
      break

    fp.write(buffer)
    written_size += len(buffer)
    progress = 'DOWNLOADING: %10d / %10d bytes (%3.1f%%)' % (written_size, filesize, written_size * 100.0 / filesize)
    progress = progress + (chr(8) * (len(progress) + 1))
    print progress,
  fp.close()
  print '\n DOWNLOADING DONE !!'


def clone_or_update(url, targetdir):
  p = '.*/(?P<name>.*).git'
  m = re.search(p, url)
  name = m.group('name')

  GITPATH = os.path.join(targetdir, name)
  if os.path.exists(GITPATH):
    os.chdir(GITPATH)
    call(['git', 'pull'])
    os.chdir(targetdir)
  else:
    call(['git', 'clone', url])



################################################################
##
################################################################

def unzip(filepath, targetdir):

  fp = open(filepath, 'rb')
  zip = zipfile.ZipFile(fp)
  namelist = zip.namelist()

  current = 1
  nextp = current
  steps = total / 100
  total = len(namelist)
  for name in namelist:
    zip.extract(name, targetdir)
    if current == nextp or current == total:
      progress = 'EXTRACTING: %5d /%5d (%3.1f%%)' % (current, total, current * 100.1 / total)
      progress = progress + (chr(8) * (len(progress) + 1))
      print progress,
      nextp += steps
    current += 1
  fp.close()

  print '\n EXTRACTING DONE !!'


def untgz(filepath, targetdir):

  tgz = tarfile.open(filepath)
  names = tgz.getnames()

  current = 1
  nextp = current
  steps = total / 100
  total = len(names)
  for name in names:
    tgz.extract(name, targetdir)
    if current == nextp or current == total:
      progress = 'EXTRACTING: %5d /%5d (%3.1f%%)' % (current, total, current * 100.1 / total)
      progress = progress + (chr(8) * (len(progress) + 1))
      print progress,
      nextp += steps
    current += 1
  tgz.close()


def extract(filepath, targetdir):

  print '#### extract [%s] file to [%s] directory' % (filepath, targetdir)

  if filepath.endswith('.zip'):
    unzip(filepath, targetdir)
  elif filepath.endswith('.tar.gz') or filepath.endswith('tgz'):
    untgz(filepath, targetdir)
  else:
    print '[%s] has unknown compressed file extension.' % filepath
    return False
  return True
