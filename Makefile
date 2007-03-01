PREFIX=/opt

LIBUNIX=.

ifndef CC
CC:=gcc
endif

CFLAGS=

ifneq ($(strip $(shell $(CC) -v 2>&1 | grep "mingw")),)
WIN32=true
endif

ifneq ($(strip $(shell $(CC) -v 2>&1 | grep -i "SunOS")),)
SOL8=true
CFLAGS+=-DSOL8
endif

TARGET=

ifdef WIN32
EXT=.exe
DEXT=.dll
LIB=	libunix.a
LIB+=	libunix_so.dll
LIBF=	-lunix
SLIBF=	-lunix_so
TARGET+= $(LIB)	
else
EXT=
DEXT=.so
TARGET2= bummer
LIB=
LIBF=
endif

TARGET += test$(EXT) pipe$(EXT) torture$(EXT) err$(EXT) misc$(EXT)

CFLAGS+=	-Wall -Werror -g -O0
#CFLAGS+=-m32
#LDFLAGS=	-m32

THREADF=
ifdef WIN32
#LDFLAGS+= -L. -Wl,--whole-archive -lsocket -Wl,--no-whole-archive -lwsock32
LDFLAGS+= -L$(LIBUNIX) $(LIBF) -lws2_32 -lkernel32
SLDFLAGS+= -L$(LIBUNIX) $(SLIBF) -lws2_32
CFLAGS+= -I$(LIBUNIX)/include
#CFLAGS+= -mno-cygwin
INSTALL= install
else
THREADF+=-lpthread
endif
LDFLAGS+=$(THREADF)

ifdef SOL8
CFLAGS+=-D_REENTRANT
LDFLAGS+=-lsocket
endif

all:	$(TARGET2) $(TARGET)

bummer:
	@echo "--- No lib to build here as libunix should only be built/used on mingw32 platforms ---"
	@echo "--- Building only the test apps for compliance purposes ---"

libunix.o: src/unix.c include/unistd.h include/sys/socket.h
	$(CC) -c -o $@ $(CFLAGS) $<

libunix.a: libunix.o
	$(AR) cru $@ $^

libunix_so.o:	src/unix.c include/unistd.h include/sys/socket.h
	$(CC) -c -o $@ -O2 -I./include $<

libunix_so$(DEXT):	libunix_so.o
	$(CC) -o $@ -shared -fPIC $^ -lws2_32 -lkernel32

misc$(EXT): misc.c $(LIB)
#	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)
	$(CC) -o $@ $(CFLAGS) $< $(SLDFLAGS)

test$(EXT): test.c $(LIB)
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

pipe$(EXT): pipe.c $(LIB)
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

torture$(EXT): torture.c $(LIB)
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

err$(EXT): err.c $(LIB)
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.o

install: all
	$(INSTALL) $(TARGET) $(PREFIX)/lib

