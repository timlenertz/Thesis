DEBUG := 0

TARGET := pcf
CXX := g++-4.9
#CXX := clang++

PACKAGES := eigen3 #opencv

CXXFLAGS := -std=c++1y -fdiagnostics-color=auto
LDFLAGS := 
LDLIBS := -lboost_iostreams -lboost_filesystem -lboost_system

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -DEIGEN_DONT_VECTORIZE -DEIGEN_DONT_ALIGN_STATICALLY -DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
else
	CXXFLAGS += -O3 -DNDEBUG -fopenmp
endif

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

%.s : %.cc
	$(CXX) $(CXXFLAGS) -S -c -o $@ $<

clean :
	rm -rf build/

mrproper : clean
	rm -rf pcf
	

-include $(DEP)
