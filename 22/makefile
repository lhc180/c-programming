PREFIX = ../

include $(PREFIX)make/env.mk
EXCLUDE = ./22.7-rle.c
include $(PREFIX)make/generic.mk

COMPILE = $(CC) -c $(CFLAGS) $<
LINK    = $(LD) $^ -o $@

all: $(EXECUTABLES) 22.7-rle

22.7-rle: 22.7-rle.o $(LIBS)/error.o
	$(LINK)

22.7-rle.o: %.o : %.c $(INCS)/error.h
	$(COMPILE)

include $(PREFIX)make/clean.mk
include $(LIBS)/makefile