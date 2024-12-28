CC = g++
CFLAGS = -Wall -Wextra -pedantic -Wno-type-limits
LDFLAGS = -lglfw -lGL -lGLEW -lglm -lm -limgui -lassimp

SRCDIR = src
ENGINE_SRCDIR = $(SRCDIR)/engine
ENGINE_HEADERDIR = $(ENGINE_SRCDIR)/header
OBJDIR = obj
TARGETDIR = bin

ENGINE_SOURCES = $(wildcard $(ENGINE_SRCDIR)/*.cpp)
ENGINE_OBJECTS = $(patsubst $(ENGINE_SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(ENGINE_SOURCES))
ENGINE_TARGET = $(TARGETDIR)/engine

TEST_SRCDIR = $(SRCDIR)/test
TEST_SOURCES = $(wildcard $(TEST_SRCDIR)/*.cpp)
TEST_OBJECTS = $(patsubst $(TEST_SRCDIR)/%.cpp, $(OBJDIR)/test_%.o, $(TEST_SOURCES))
TEST_TARGET = $(TARGETDIR)/test

all: $(ENGINE_TARGET) $(TEST_TARGET) run

engine: $(ENGINE_TARGET)

test: $(TEST_TARGET)

# target building
$(ENGINE_TARGET): $(ENGINE_OBJECTS) | $(TARGETDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJECTS) | $(TARGETDIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# object building
$(OBJDIR)/%.o: $(ENGINE_SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(ENGINE_HEADERDIR) -c $< -o $@

$(OBJDIR)/test_%.o: $(TEST_SRCDIR)/%.cpp | $(OBJDIR)
	$(CC) $(CFLAGS) -I$(ENGINE_HEADERDIR) -c $< -o $@

# directories
$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGETDIR):
	mkdir -p $(TARGETDIR)

clean:
	rm -rf $(OBJDIR)
	rm -rf $(TARGETDIR)

count:
	cloc --exclude-list-file=.clocignore .

run:
	./bin/engine

.phony: all clean count run engine test
