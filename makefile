CC=gcc
CFLAGS=-c -Wall -g -Wno-unused-variable
LDFLAGS=-lm

EXECUTABLE=trace

INC=-Iinc
SRCDIR=src
OBJDIR=obj
SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.c=.o)))

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir obj -p
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm $(OBJECTS)
	rm $(EXECUTABLE)
	rm $(OBJECTS:.o=.d)

count:
	@find . -name '*.c' -or -name '*.h' | xargs wc -l
