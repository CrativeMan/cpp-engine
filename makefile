CC = g++
CFLAGS = -Wall -Wextra -pedantic -Wno-type-limits
LDFLAGS = -lglfw -lGL -lGLEW -lglm -lm -limgui -lassimp
SRCDIR = src
OBJDIR = obj
TARGETDIR = bin
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
TARGETMAIN = $(TARGETDIR)/main

all: $(TARGETMAIN)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGETDIR):
	mkdir -p $(TARGETDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGETMAIN): $(OBJS) | $(TARGETDIR)
	@echo "Building engine..."
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Finished building engine."

clean:
	rm -rf $(OBJDIR)
	rm -rf $(TARGETDIR)

count:
	cloc --exclude-list-file=.clocignore .

run:
	./bin/main