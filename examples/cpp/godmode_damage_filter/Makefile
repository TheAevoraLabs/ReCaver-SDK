CXX ?= g++
CXXFLAGS ?= -O2 -fPIC -std=c++14 -Wall -Wextra
INCLUDES = -I../../../include

TARGET = libgodmode_damage_filter.so
SRCS = src/filter_main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -shared $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)
