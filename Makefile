#
# Copyright 2014, Zuoyi Jian <jzuoyi@126.com>
#
# This file is part of cpgiscan
#


#
# Makefile for cpgiscan
#

CC = gcc
CPP = g++
CXX = $(CPP)


RELEASE_FLAGS = -std=c++11 -Wall
PTHREAD_LIB = -lpthread
LIBS = $(PTHREAD_LIB)

CPGISCAN_CPPS = $(wildcard CpGI/*.cc)

cpgiscan: $(CPGISCAN_CPPS)
	$(CXX) $(RELEASE_FLAGS) $(LIBS) -o $@ $^

.PHONY:clean

clean:
	rm -f cpgiscan
