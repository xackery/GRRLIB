# Quick'n'dirty makefile [BC] v2
DEVKITPRO := /Users/leroy_k/devkitPro
DEVKITPPC := $(DEVKITPRO)/devkitPPC



NULLSTR :=
PWD     := $(subst $(NULLSTR) ,\ ,$(shell pwd))

PREFIX	:= $(DEVKITPPC)/bin/powerpc-eabi-
CC		:=	$(PREFIX)gcc
AR		:=	$(PREFIX)ar

LIBOGC_INC	:=	$(DEVKITPRO)/libogc/include
LIBOGC_LIB	:=	$(DEVKITPRO)/libogc/lib/cube

INCLUDE	:= -I../lib/jpeg -I../lib/pngu -I../lib/freetype/include -I$(PWD) -I$(LIBOGC_INC)
MACHDEP := -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float
CFLAGS  := -O2 -Wall $(MACHDEP) $(INCLUDE)

LIB		:= grrlib
CFILES	:= $(wildcard *.c)
OFILES	:= $(CFILES:.c=.o)
ARC		:= lib$(LIB).a
HDR		:= $(LIB).h
INL		:= $(wildcard $(LIB)/*.h)

all : $(OFILES)
	$(AR) -r $(ARC) $(OFILES)

clean :
	rm -f $(OFILES) $(ARC)

install :
	mkdir -p  $(LIBOGC_LIB)  $(LIBOGC_INC)  $(LIBOGC_INC)/grrlib
	cp -f $(ARC) $(LIBOGC_LIB)/
	cp -f $(HDR) $(LIBOGC_INC)/
	cp -f $(INL) $(LIBOGC_INC)/grrlib

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
