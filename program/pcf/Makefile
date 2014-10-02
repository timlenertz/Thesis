TARGET := pcf
CXX := g++
LN := ln

PACKAGES := eigen3

CXXFLAGS := -std=c++11 -g
LDFLAGS := 
LDLIBS := 

CXXFLAGS += $(shell pkg-config --cflags $(PACKAGES))
LDLIBS += $(shell pkg-config --libs $(PACKAGES))

CPP := $(shell find ./src -name '*.cc')
OBJ := $(patsubst %.cc,build/%.o,$(CPP))
DEP := $(patsubst %.cc,build/%.d,$(CPP))


all : $(TARGET)
	

$(TARGET) : $(OBJ)
	mkdir -p lib/
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)	

build/%.o : %.cc
	mkdir -p $(dir $@) && \
	$(CXX) $(CXXFLAGS) -c -o $@ $< -MMD

clean :
	rm -rf build/

mrproper : clean
	rm -rf pcf


-include $(DEP)
