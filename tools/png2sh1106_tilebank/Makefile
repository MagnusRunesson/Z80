CC=gcc
CXX=clang++
#CFLAGS= -O0 -g -std=c++11 -mmacosx-version-min=10.11 -Wextra -Wall -F/Library/Frameworks -I../include -I-../include
CFLAGS= -O0 -g -std=c++11 -mmacosx-version-min=10.9 -F/Library/Frameworks -I../include -I-../include
LIBS=-framework SDL2 -framework SDL2_image -lc++ -lc
LDFLAGS= -macosx_version_min 10.9 -framework SDL2 -framework SDL2_image


CFILES=png2sh1106_tilebank.cpp

OBJS=$(CFILES:.cpp=.o)

all: build install

install: build
	sudo cp png2sh1106_tilebank /usr/local/bin

build: png2sh1106_tilebank.o
	$(LD) $(LDFLAGS) -o png2sh1106_tilebank $(LIBS) $^

%.o: %.cpp
	$(CXX) $(CFLAGS) -c -o $@  $<

clean:
	rm -f png2sh1106_tilebank $(OBJS) *.o
