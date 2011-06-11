import sys
import os
import struct
import select

import vmci

CONNECTION_BACKLOG = 10

def vmci_hello(cid, port):
    af_vmci = vmci.af_vmci
    s = vmci.socket(af_vmci, vmci.SOCK_STREAM)
    # '<q' means little endian int64_t
    vmci_buffer_size = 32768
    s.setsockopt(af_vmci, vmci.SO_VMCI_BUFFER_SIZE, struct.pack('<q', vmci_buffer_size))
    t = s.getsockopt(af_vmci, vmci.SO_VMCI_BUFFER_SIZE, 8)
    t = struct.unpack('<q', t)[0]

    if t != vmci_buffer_size:
        assert(0)

    s.connect((cid, port))
    s.sendall('hello')

def program_name():
    return os.path.basename(sys.argv[0])

def print_usage_exit():
    sys.stderr.write('usage: %s CID:PORT\n' % (program_name(),))
    sys.exit(2)

def main(args):
    try:
        (cid, port) = [ int(x) for x in (args[0].split(':')) ]
    except (IndexError, ValueError):
        print_usage_exit()

    vmci_hello(cid, port)

if __name__ == '__main__':
    main(sys.argv[1:])
