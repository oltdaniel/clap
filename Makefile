# Define compiler options
CC := clang
CFLAGS := -g
LIB :=
INC := -I include

# Define directories
SRCDIR := src
BUILDDIR := build
TARGET := bin/clap

# Find files
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Link everything together
$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@echo " $(CC) $^ -o $(TARGET) $(LIB)"; $(CC) $^ -o $(TARGET) $(LIB)

# Compile each source file
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Clean up the project
clean:
	@echo " Cleaning...";
	@echo " $(RM) $(BUILDDIR)/* $(TARGET)"; $(RM) $(BUILDDIR)/* $(TARGET)

.PHONY: clean
