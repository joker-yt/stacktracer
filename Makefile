TARGET=test_app
TARGET_TRACER=libtracer.so
CPPFLAGS=-std=c++11
CPPFLAGS+=-fPIC
CPPFLAGS+=-ldl
CPPFLAGS+=-g
CPPFLAGS+=-DDEBUG
SRC= main.cpp
TRACER_SRC= tracer.cpp
CXX=g++

all:$(TARGET)
	$(CXX) -o $< $(SRC) $(CPPFLAGS) -finstrument-functions -rdynamic

$(TARGET):


tracer:$(TARGET_TRACER)
	$(CXX) -o $< $(TRACER_SRC) $(CPPFLAGS) -shared

$(TARGET_TRACER):

exec:
	LD_PRELOAD=./$(TARGET_TRACER) ./$(TARGET)

png:
	seqdiag output.dot

sample-png:
		seqdiag sample.dot
