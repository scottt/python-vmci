#!/usr/bin/env python
import os
from setuptools import setup, Extension

include_dirs = list()
for d in [ '/usr/lib/vmware/include/vmci',
           '/usr/lib/vmware-tools/include/vmci',
          r'C:\Program Files\Common Files\VMware\Drivers\vmci\sockets\include' ]:
    if os.path.isdir(d):
        include_dirs.append(d)
libraries = list()
if os.name == 'nt':
    libraries.append('ws2_32')

setup(name='vmci',
    version='1.0',
    py_modules=['vmci'],
    ext_modules=[Extension('_vmci', ['vmcimodule.c'], include_dirs=include_dirs, libraries=libraries)],
)
