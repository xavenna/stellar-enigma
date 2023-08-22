# This makefile was created with help from the following stackoverflow answer:
# https://stackoverflow.com/a/23418196
EXE = stellar
CXX = g++
CPPFLAGS = -Wall -Wextra -fexceptions -std=c++17 -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused -Wpedantic # -Werror


ifeq ($(OS),Windows_NT)
SFINCDIR = -IC:\SFML\include
SFLIBDIR = -LC:\SFML\lib
LLIB = -lsfml-graphics-d -lfreetype -lsfml-window-d -lopengl32 -lgdi32 -lsfml-audio-d -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lsfml-system-d -lwinmm -lsfml-main-d
EXE += .exe
else
LLIB = -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
LFLAGS +=  -no-pie
endif


ifeq (1, $(REL))
  CPPFLAGS += -O2 -s -DNDEBUG
  OBJ_DIR = ./obj/release
else
  CPPFLAGS += -g -O0
  OBJ_DIR = ./obj/debug
endif

SRCS = $(wildcard src/*.cpp)
df = $(OBJ_DIR)/$(*F)
AUTODEPS:=$(patsubst src/%.cpp, $(OBJ_DIR)/%.d, $(SRCS))
OBJS:=$(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
LIBOBJS := $(filter-out custom.o main.o, $(OBJS))


.PHONY : all clean tilde debug release remake lib

all: $(EXE)

lib : libstellar.a

libstellar.a : $(LIBOBJS)
	ar rcs $@ $^

$(EXE): $(OBJS)
	$(CXX) $(SFLIBDIR) $(LFLAGS) -o $@ $^ $(LLIB)

$(OBJ_DIR)/%.o: src/%.cpp
	@$(CXX) -MM -MP -MT $(df).o -MT $(df).d $(CPPFLAGS) $< > $(df).d
	$(CXX) $(SFINCDIR) $(CPPFLAGS) -c $< -o $@

-include $(AUTODEPS)

remake: clean all

clean :
	rm -f obj/debug/*.o obj/release/*.o obj/debug/*.d obj/release/*.d ./stellar
tilde :
	rm *~
