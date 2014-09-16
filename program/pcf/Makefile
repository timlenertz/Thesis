CXX := g++
LN := ln

DEPLOY := 0
VARIANT := dynamic
ARCH := -arch i386 -arch x86_64

TARGET_DYNAMIC := lib/libdypc.dylib
TARGET_FRAMEWORK := Frameworks/dypc.framework

FRAMEWORK_VERSION := A

BUILD_CONFIG_HPP := include/dypc/build_config.h

define BUILD_CONFIG
// Automatically generated during build process


endef

CXXFLAGS := -std=c++11 -fno-common -fvisibility=hidden $(ARCH) -DDYPC_EXPORTS -iquote include/dypc -include src/common.h
LDFLAGS := $(ARCH)
ifeq ($(DEPLOY), 1)
	CXXFLAGS += -O3 -DNDEBUG
else
	CXXFLAGS += -g
endif

ifeq ($(VARIANT), dynamic)
	TARGET := $(TARGET_DYNAMIC)
	CXXFLAGS += -fPIC
	LDFLAGS += -dynamiclib
	BUILD_CONFIG +=\#define DYPC_SHARED_LIBRARY
else ifeq ($(VARIANT), framework)
	TARGET := $(TARGET_FRAMEWORK)
	CXXFLAGS += -fPIC
	LDFLAGS += -dynamiclib
	BUILD_CONFIG +=\#define DYPC_SHARED_LIBRARY
endif endif

export BUILD_CONFIG

CXXFLAGS += -I"external/include" -I"osx/external/include"
LDFLAGS += -L"external/lib" -L"osx/external/lib"

LDLIBS += -lm -lz -lhdf5 -lhdf5_cpp

CPP := $(shell find ./src -name '*.cc')
INTERFACE_HPP := $(shell find ./src -name '*.h' -not -path "./src/impl/*")
OBJ := $(patsubst %.cc,build/%.o,$(CPP))
DEP := $(patsubst %.cc,build/%.d,$(CPP))

INCLUDE_HPP := $(patsubst ./src/%.h,include/dypc/%.h,$(INTERFACE_HPP))


all : $(BUILD_CONFIG_HPP) $(TARGET)
	

$(TARGET_FRAMEWORK) : $(TARGET_DYNAMIC)
	mkdir -p $(TARGET_FRAMEWORK)/Versions/$(FRAMEWORK_VERSION)/{Headers,Resources}
	ln -s $(FRAMEWORK_VERSION) $(TARGET_FRAMEWORK)/Versions/Current
	cp $(TARGET_DYNAMIC) $(TARGET_FRAMEWORK)/Versions/Current/dypc
	cp -r include/* $(TARGET_FRAMEWORK)/Versions/Current/Headers/
	ln -s Versions/Current/Headers $(TARGET_FRAMEWORK)/Headers
	ln -s Versions/Current/Resources $(TARGET_FRAMEWORK)/Resources
	ln -s Versions/Current/dypc $(TARGET_FRAMEWORK)/dypc
	rm -rf include/ lib/

$(TARGET_DYNAMIC) : $(OBJ) $(INCLUDE_HPP)
	mkdir -p lib/
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)	

includes : $(INCLUDE_HPP)
	

include/dypc/%.h : src/%.h
	mkdir -p $(dir $@)
	cp $< $@

$(BUILD_CONFIG_HPP) :
	mkdir -p $(dir $@)
	echo "$$BUILD_CONFIG" > $@

build/%.o : %.cc
	mkdir -p $(dir $@) && \
	$(CXX) $(CXXFLAGS) -c -o $@ $< -MMD

clean :
	rm -rf build/

mrproper : clean
	rm -rf include/ lib/ Frameworks/


-include $(DEP)
