# Makefile for the writer application
#
# Native build:  make
# Cross build:   make CROSS_COMPILE=aarch64-none-linux-gnu-
# Clean:         make clean

CROSS_COMPILE ?=

# Use $(CROSS_COMPILE)gcc unless CC was set explicitly on the command
# line or in the environment (make's built-in default for CC is "cc").
ifeq ($(origin CC),default)
CC = $(CROSS_COMPILE)gcc
endif

CFLAGS  ?= -g -Wall -Werror
LDFLAGS ?=

TARGET := writer
SRCS   := writer.c
OBJS   := $(SRCS:.c=.o)

.PHONY: all default clean

all: $(TARGET)

default: all

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o
