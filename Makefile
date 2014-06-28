# Build path
BUILD_DIR=build/

# Build flags and libs
CXXFLAGS = -MMD -Wall -Werror -pthread
LDFLAGS = -L/usr/local/lib/
LDLIBS = -lm

# Platform configuration
UNAME := $(shell uname -s)

ifeq ($(UNAME), Linux)
	CXX = g++
	LD = g++
	CXXFLAGS += -std=c++0x
	LDLIBS += -lpthread -lGL -lGLU -lglut
endif

ifeq ($(UNAME), Darwin)
	CXX = c++
	LD = c++
	CXXFLAGS += -std=c++11 -Wno-deprecated -I/usr/local/include 
	CXXFLAGS += -stdlib=libc++
	LDFLAGS += -stdlib=libc++
	LDLIBS += -framework OpenGL -framework glut
endif

# Include from source dirs
CXXFLAGS += -Isource/

# Build type configuration
CXXFLAGS += -g -O3 -DDEVELOPMENT
LDFLAGS += -g -O3

# Source build rule
$(BUILD_DIR)%.o: source/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

# All generated directories
ALL_DIRS = $(BUILD_DIR)

# Application sources
SOURCES = main
SOURCES += OpenGL
SOURCES += GlutApplication
SOURCES += GlModel
SOURCES += GlMeshBuilder

# Application objects
OBJECTS =  $(patsubst %, $(BUILD_DIR)%.o, $(SOURCES))

# Default targets
default: $(ALL_DIRS) $(BUILD_DIR)application.bin run

# Build all
all: default

# Application linkage
$(BUILD_DIR)application.bin: $(OBJECTS) 
	$(LD) $(LDFLAGS) -o $@  $(OBJECTS) $(LDLIBS)

# Include dependencies for all .c and .cpp files
-include $(BUILD_DIR)*.d

# Diractories generation
$(ALL_DIRS):
	mkdir -p $@
	
# Clean build
clean:
	rm -rf $(BUILD_DIR)

# Run application
run: $(BUILD_DIR)application.bin
	./$(BUILD_DIR)application.bin
