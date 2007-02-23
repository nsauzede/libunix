PREFIX=/opt

LIBUNIX=.

ifndef CC
CC:=gcc
endif
ifneq ($(strip $(shell $(CC) -v 2>&1 | grep "mingw")),)
WIN32=true
endif

TARGET=

ifdef WIN32
TARGET+=	libunix.a
EXT=.exe
else
EXT=
TARGET2= bummer
endif

TARGET += test pipe

CFLAGS=	-Wall -Werror -g -O0
#CFLAGS+=-m32
#LDFLAGS=	-m32

THREADF=
ifdef WIN32
#LDFLAGS+= -L. -Wl,--whole-archive -lsocket -Wl,--no-whole-archive -lwsock32
LDFLAGS+= -L$(LIBUNIX) -lunix -lws2_32
CFLAGS+= -I./include
#CFLAGS+= -mno-cygwin
INSTALL= install
else
THREADF+=-lpthread
endif


all:	$(TARGET) $(TARGET2)

bummer:
	@echo "--- Nothing to do here as libunix should only be built/used on mingw32 platforms ---"
	@touch libsocket.a

libunix.o: src/unix.c include/unistd.h include/sys/socket.h
	$(CC) -c -o $@ $(CFLAGS) $<

libunix.a: libunix.o
	$(AR) cru $@ $^

test: test.c
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

pipe: pipe.c
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

clean: $(TARGET2)
	$(RM) $(TARGET) *.o

install: all
	$(INSTALL) $(TARGET) $(PREFIX)/lib

