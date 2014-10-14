TARGET := pcf
CXX := g++-4.9
#CXX := clang++

PACKAGES := eigen3 opencv

CXXFLAGS := -std=c++1y -fdiagnostics-color=auto -mmacosx-version-min=10.9
LDFLAGS := 
LDLIBS := -lboost_iostreams -lboost_filesystem -lboost_system

CXXFLAGS += -O3 -g -fopenmp

#ifeq ($(DEPLOY), 1)
#	CXXFLAGS += -O3  -DNDEBUG -g
#else
#	CXXFLAGS += -g
#endif

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
