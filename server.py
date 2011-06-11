import sys
import struct
import select

import vmci

CONNECTION_BACKLOG = 10

af_vmci = vmci.af_vmci
listen_sock = vmci.socket(af_vmci, vmci.SOCK_STREAM)
# '<q' means little endian int64_t
vmci_buffer_size = 32768
listen_sock.setsockopt(af_vmci, vmci.SO_VMCI_BUFFER_SIZE, struct.pack('<q', vmci_buffer_size))
t = listen_sock.getsockopt(af_vmci, vmci.SO_VMCI_BUFFER_SIZE, 8)
t = struct.unpack('<q', t)[0]

if t != vmci_buffer_size:
    assert(0)

listen_sock.bind((vmci.VMADDR_CID_ANY, vmci.VMADDR_PORT_ANY))
cid = vmci.get_local_cid()
myaddr = listen_sock.getsockname()
sys.stderr.write('server (cid, port): %d:%d\n' % myaddr)

while 1:
    listen_sock.listen(CONNECTION_BACKLOG)
    (conn, client_addr) = listen_sock.accept()
    (rfds, wfds, xfds) = select.select([conn], [], [])
    data = conn.recv(1024)
    sys.stderr.write('received: %r\n' % (data,))
