DEBUG := 0

TARGET := pcf
CXX := g++-4.9
#CXX := clang++

PACKAGES := eigen3 #opencv

CXXFLAGS := -std=c++1y -fdiagnostics-color=auto -arch x86_64
LDFLAGS := 
LDLIBS := -lboost_iostreams -lboost_filesystem -lboost_system

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g -DEIGEN_DONT_VECTORIZE -DEIGEN_DONT_ALIGN_STATICALLY -DEIGEN_DISABLE_UNALIGNED_ARRAY_ASSERT
else
	CXXFLAGS += -O3 -DNDEBUG #-fopenmp
endif

CXXFLAGS += $(shell pkg-config --cflags $(PACKAGES))
LDLIBS += $(shell pkg-config --libs $(PACKAGES))

CPP := $(shell find ./src -name '*.cc' -not -path "./src/main.cc")
OBJ := $(patsubst %.cc,build/%.o,$(CPP))
DEP := $(patsubst %.cc,build/%.d,$(CPP))

TEST_CPP := $(shell find ./test -name '*.cc')
TEST_OBJ := $(patsubst %.cc,%.o,$(TEST_CPP))

TEST_TARGET := ./test/test
TEST_CXXFLAGS := $(CXXFLAGS) $(shell pkg-config --cflags cppunit)
TEST_LDFLAGS := $(LDFLAGS)
TEST_LDLIBS := $(LDLIBS) $(shell pkg-config --libs cppunit)


all : $(TARGET)
	

$(TARGET) : $(OBJ) ./build/src/main.o
	mkdir -p lib/
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LDLIBS)	

build/%.o : %.cc
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $< -MMD

%.s : %.cc
	$(CXX) $(CXXFLAGS) -S -c -o $@ $<

test/%.o : test/%.cc
	$(CXX) $(TEST_CXXFLAGS) -c -o $@ $<

$(TEST_TARGET) : $(TEST_OBJ) $(OBJ)
	$(CXX) $(TEST_CXXFLAGS) $(TEST_LDFLAGS) -o $@ $(TEST_OBJ) $(OBJ) $(TEST_LDLIBS)

test : $(TEST_TARGET)
	./test/test

clean :
	rm -rf build/ $(TEST_OBJ) $(TEST_TARGET)

mrproper : clean
	rm -rf pcf
	

.PHONY : all test clean mrproper

-include $(DEP) ./build/src/main.d
