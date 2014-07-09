# Change to 1 to build windows test programs
BUILD_WINDOWS_TEST_PROGRAMS := 0

VMCI_CFLAGS := $(shell ./vmci-cflags)
CFLAGS := -Wall -Os -g $(VMCI_CFLAGS)

.PHONY: all clean

PROGRAMS := server client

ifeq ($(BUILD_WINDOWS_TEST_PROGRAMS),1)
PROGRAMS += server.exe client.exe
endif

DERIVED_FILES := $(PROGRAMS) _vmci.so

all: $(DERIVED_FILES)
_vmci.so: vmcimodule.c
	$(CC) $+ $(CFLAGS) $(LDFLAGS) -o $@
_vmci.so: CFLAGS += -fno-strict-aliasing -shared -fPIC $(shell pkg-config python --cflags)

%: %.c sock-posix.c
	$(CC) $+ $(CFLAGS) $(LDFLAGS) -o $@
%: LDFLAGS += $(VMCI_LIBS)

%.exe: %.c sock-windows.c
	$(CC) $+ $(CFLAGS) $(LDFLAGS) -o $@
%.exe: CC := i686-pc-mingw32-gcc
%.exe: LDFLAGS += $(VMCI_LIBS) -lws2_32

clean:
	rm -f $(DERIVED_FILES)
