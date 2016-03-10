
# Project: sttly
# Author:  Beth Boose
#
# Makefile to build sttly
# CFLAGS:
#   -Wall  turns on a lot of useful warning messages
#   -Wextra turns on more warning messages missed by -Wall
#   -std=gnu99 use C99 for compiling
#   -DDEBUG set to output debug statements
#
# VARIABLES:
#   TGT - name of the executable
#   SRC - class files files
#   DEPS - header files
#   OBJS -.o files generated from class files
#

CFLAGS = -Wall -Wextra -std=gnu99 -DEBUG
TGT = sttyl
SRC = $(wildcard *.c)
DEPS := $(wildcard *.h)
OBJS := $(SRC: .c=.o)
	
$(TGT): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)
	
clean:
	rm -f *.o core $(TGT)
	