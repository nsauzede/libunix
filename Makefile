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
TARGET+=	libunix.a
EXT=.exe
else
EXT=
TARGET2= bummer
endif

TARGET += test$(EXT) pipe$(EXT) torture$(EXT)

CFLAGS+=	-Wall -Werror -g -O0
#CFLAGS+=-m32
#LDFLAGS=	-m32

LIB=	libunix.a
LIBF=	-lunix

THREADF=
ifdef WIN32
#LDFLAGS+= -L. -Wl,--whole-archive -lsocket -Wl,--no-whole-archive -lwsock32
LDFLAGS+= -L$(LIBUNIX) $(LIBF) -lws2_32
CFLAGS+= -I./include
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

$(LIB): libunix.o
	$(AR) cru $@ $^

test$(EXT): test.c $(LIB)
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

pipe$(EXT): pipe.c $(LIB)
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

torture$(EXT): torture.c $(LIB)
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

clean:
	$(RM) $(TARGET) *.o

install: all
	$(INSTALL) $(TARGET) $(PREFIX)/lib

