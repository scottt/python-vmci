python-vmci
=======

This repo contains Python standard library modules patched with VMCI support:

 * `vmci`: socket with VMCI support
 * `asyncvmci`: asyncore with VMCI support
 * `portfwd-vmci` is a VMCI to TCP port forwarder that uses asyncore

These patched modules in this repository are all licensed under the same very liberal terms as Python itself. See [LICENSE](LICENSE).
It only supports Python 2 at the moment but should be portable to Python 3 with minimal effort. Patches Welcome!

Compiling
-------
On Linux, make sure you have the bluetooth headers installed. The package is `bluez-devel` on Fedora and Red Hat and `libbluetooth-dev` on Ubuntu and Debian.

On Windows, make sure you have Microsoft Visual C++ Compiler for Python 2.7 installed (http://aka.ms/vcpython27).

Build on a machine with VMWare installed


    cd python-vmci
    python setup.py build
    python setup.py bdist_wheel # optional

Source Layout
-------------

 - [vmci.py](vmci.py) and [vmcimodule.c](vmcimodule.c) are Python 2.7's standard `socket` module patched with VMCI support
 - [asyncvmci.py](asyncvmci.py) is Python 2.7's asyncore patched.
 - [server.c](server.c) and [client.c](client.c) are minimal VMCI test programs to help diagnose communication problems
