DEVKITPRO := C:/devkitPro
DEVKITPPC := $(DEVKITPRO)/devkitPPC

PREFIX	:= $(DEVKITPPC)/bin/powerpc-eabi-
CC		:=	$(PREFIX)gcc
AR		:=	$(PREFIX)ar

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/cube

INCLUDE	:= 
MACHDEP := -DGEKKO -mgcn -mcpu=750 -meabi -mhard-float
CFLAGS  := -O2 $(MACHDEP) $(INCLUDE)

LIB 	:= jpeg
CFILES	:= $(wildcard *.c)
OFILES	:= $(CFILES:.c=.o)
ARC 	:= lib$(LIB).a
HDR 	:= jpeglib.h jconfig.h jmorecfg.h

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)

install :
	mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)
	cp -f $(ARC) $(LIBOGC_LIB)/
	cp -f $(HDR) $(LIBOGC_INC)/

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
