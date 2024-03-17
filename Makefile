CC = gcc
CFLAGS = -ansi -g -std=c99

# List of source files
SRCS = main.c Movie.c

BIN_DIR = bin

# Generate a list of object files in the bin directory
OBJS = $(patsubst %.c,$(BIN_DIR)/%.o,$(SRCS))

# Target executable
TARGET = cs240StreamingServicePhaseb
# Default target
all: $(TARGET)

# Linking step: Create the executable from object files
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation step: Create object files in the bin directory
$(BIN_DIR)/%.o: %.c | $(BIN_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Create the bin directory if it doesn't exist
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean up intermediate and executable files
clean:
	rm -rf $(BIN_DIR) $(TARGET)