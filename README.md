python-vmci
=======

This repo contains Python standard library modules patched with VMCI support:

 * `vmci`: socket with VMCI support
 * `asyncvmci`: asyncore with VMCI support
 * `portfwd-vmci` is a VMCI to TCP port forwarder that uses asyncore

Compiling
-------
On Linux, make sure you have the bluetooth headers installed. The package is `bluez-devel` on Fedora and Red Hat and `libbluetooth-dev` on Ubuntu and Debian.

Build on a machine with VMWare installed


    # check that ./vmci-cflags succeeds
    cd python-vmci
    make

Source Layout
-------------

 - `vmci.py` and `vmcimodule.c` are Python's standard `socket` module patched with VMCI support
 - `asyncvmci.py` is `asyncore` patched.
 - `server.c` and `client.c` are minimal VMCI test programs to help diagnose communication problems
