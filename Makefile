# based on code from https://stackoverflow.com/a/20830354
# but modified to use c++ with the SFML library
CFLAGS = -Wall -fexceptions -std=c++11
LLIB = -lsfml-graphics-d -lfreetype -lsfml-window-d -lopengl32 -lgdi32 -lsfml-audio-d -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lsfml-system-d -lwinmm -lsfml-main-d
INCD = -ID:/usr/inc -Iinclude
LNKD = -LD:/lib/SFML-2.5.1
HDIR = include
CC = g++

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADER = $(wildcard include/*.h)
EXE = stellar.exe  #this should be changed to the desired program name

# Debug build settings

DBGDIR = debug
DBGEXE = $(DBGDIR)/$(EXE)
DBGOBJS = $(addprefix $(DBGDIR)/, $(OBJS))
DBGCFLAGS = -g -O0

# Release build settings
RELDIR = release
RELEXE = $(RELDIR)/$(EXE)
RELOBJS = $(addprefix $(RELDIR)/, $(OBJS))
RELCFLAGS = -O3 -DNDEBUG

.PHONY: all clean debug prep release remake

# Default build
all: debug

#
# Debug rules
#


debug: $(DBGEXE)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(DBGCFLAGS) $(INCD) -c $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
include $(sources:.c=.d)

$(DBGEXE): $(DBGOBJS)
	$(CC) $(LNKD) -o $(DBGEXE) $^ $(LLIB)
$(DBGDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) $(DBGCFLAGS) $(INCD) -c $< -o $@

#
# Release rules
#
release: $(RELEXE)

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $(RELCFLAGS) $(INCD) -c $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
include $(sources:.c=.d)

$(RELEXE) : $(RELOBJS)
	$(CC) $(LNKD) -o $(RELEXE) $^ $(LLIB)
$(RELDIR)/%.o : %.cpp
	$(CC) $(CFLAGS) $(RELCFLAGS) $(INCD) -c $< -o 4@

#
# Other rules
#
prep:
	@mkdir -p $(DBGDIR) $(RELDIR) $(HDIR)

remake: clean all

clean:
	rm -f $(RELEXE) $(RELOBJS) $(DBGEXE) $(DBGOBJS)
