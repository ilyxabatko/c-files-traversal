# Define the compiler and the flags
CC = gcc
CFLAGS = -Wall -I/opt/homebrew/Cellar/json-c/0.17/include
LDFLAGS = -L/opt/homebrew/Cellar/json-c/0.17/lib -ljson-c

# Define the target executable and source files
TARGET = file_traversal
SRCS = file_traversal.c

# The first rule is the default rule
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)
