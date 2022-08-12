CC = g++
CFLAGS  = -o 2 -Wall
# compiler flags:
#  -o 2   - this flag optimizes the binary
#  -g     - this flag adds debugging information to the executable file
#  -Wall  - this flag is used to turn on most compiler warnings

# The build target 
TARGET = main
 
all: $(TARGET)
 
$(TARGET): $(TARGET).cpp
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).cpp

clean:
	$(RM) $(TARGET)
