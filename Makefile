# This makefile was created with help from the following stackoverflow answer:
# https://stackoverflow.com/a/23418196

### Instructions:

# For a full-source build, just run make in the home directory.
#
# To build as a dynamic library, set DYNAMIC to 1


# For a simple build, place your custom main.cpp in the src/ directory and run make.

# To build in SDK mode (if any new objects are added), set SDK to 1
#  Make your changes to obj-container.cpp, and run make.

# To build doxygen documentation, ensure doxygen is installed, and run make doc. Output
#  will be written to doxygen.log
#
#  For a release build, run make with REL=1 as an argument

DYNAMIC = 0

SDK = 0

#if 1, use time to seed rng, else use random_device
RAND_TIME = 0

# controls if raw keyboard state or os-provided events are used for input.
RAW_IN = 1

# Initially defined up here so the following lines work properly
CPPFLAGS = -fexceptions -std=c++17

# Play window size, in nodes
CPPFLAGS += -DWINDOW_WIDTH=16 -DWINDOW_HEIGHT=12

# Framerate definition
CPPFLAGS += -DFRAMERATE=30


ifeq ($(SDK), 1)
BASE_LIB := libstellar-sdk
else
BASE_LIB := libstellar
endif



EXE := stellar
LIBS = libstellar.a libstellar-sdk.a libstellar.so libstellar-sdk.so
CXX = g++


ifeq ($(RAW_IN), 1)
CPPFLAGS += -DSE_RAW_IN
endif

ifeq ($(RAND_TIME), 1)
CPPFLAGS += -DRAND_USE_TIME
endif

# Warning flags

CPPFLAGS += -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef  -Wpedantic # -Werror -Wno-unused


LFLAGS = -L.

ifeq ($(OS),Windows_NT)
SFINCDIR = -IC:\SFML\include
SFLIBDIR = -LC:\SFML\lib
LLIB = -lstellar -lsfml-graphics-d -lfreetype -lsfml-window-d -lopengl32 -lgdi32 -lsfml-audio-d -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lsfml-system-d -lwinmm -lsfml-main-d
EXE := $(EXE).exe
LIB := $(BASE_LIB).a

ifeq ($(DYNAMIC),1)
	echo Error: dynamic linking is not currently supported on Windows. Building static instead
endif

else

ifeq ($(DYNAMIC),1)
CPPFLAGS += -fpic
LLIB = -lstellar -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -ljson11
LIB := $(BASE_LIB).so
LFLAGS += -Wl,-rpath=.
else
LLIB = -lstellar -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system -ljson11
LFLAGS +=  -no-pie
LIB := $(BASE_LIB).a
endif

endif


ifeq (1, $(REL))
  CPPFLAGS += -O2 -s -DNDEBUG
  OBJ_DIR = ./obj/release
else
  CPPFLAGS += -g -O0
  OBJ_DIR = ./obj/debug
endif

SRCS = $(wildcard src/*.cpp)
HEADERS = $(wildcard src/*.h) $(wildcard src/*.hpp)
df = $(OBJ_DIR)/$(*F)
AUTODEPS:=$(patsubst src/%.cpp, $(OBJ_DIR)/%.d, $(SRCS))
OBJS:=$(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
LIBOBJS := $(filter-out $(OBJ_DIR)/main.o, $(OBJS))
ifeq (1, $(SDK))
LIBOBJS := $(filter-out $(OBJ_DIR)/obj-container.o, $(LIBOBJS))
# Any other files that become user-editable in SDK mode go here
endif


# libfreeobjs is all the object files not compiled into the library
LIBFREEOBJS := $(OBJ_DIR)/main.o

ifeq (1, $(SDK))
	# any code that needs modified by an advanced user goes here.
	LIBFREEOBJS += $(OBJ_DIR)/obj-container.o
endif

.PHONY : all clean remake lib doc

all: $(EXE)

lib : $(LIB)

$(BASE_LIB).a : $(LIBOBJS)
	ar rcs $(LIBFLAGS) $@ $^

$(BASE_LIB).so : $(LIBOBJS)
	$(CXX) -shared -o $@ $^

doc : $(HEADERS)
	@doxygen >> doxygen.log 2>&1
	@echo Generating Documentation

$(EXE): $(LIBFREEOBJS) $(LIB)
	$(CXX) $(SFLIBDIR) $(LFLAGS) -o $@ $(OBJ_DIR)/main.o $(LLIB)

$(OBJ_DIR)/%.o: src/%.cpp
	@$(CXX) -MM -MP -MT $(df).o -MT $(df).d $(CPPFLAGS) $< > $(df).d
	$(CXX) $(SFINCDIR) $(CPPFLAGS) -c $< -o $@

-include $(AUTODEPS)

remake: clean all

clean :
	rm -f obj/debug/*.o obj/release/*.o obj/debug/*.d obj/release/*.d $(EXE) $(LIBS)
