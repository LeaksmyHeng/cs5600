# Define the Java compiler and compiler flags
JAVAC = javac
JFLAGS = -d

# Define the source and output directories
SRCDIR = src
OUT = out

# List of source files
CLASSES = \
	./src/BoundedBuffer.java \
	./src/generateThreadID.java \
	./src/Main.java

default: classes

classes: $(CLASSES:.java=.class)

# Run the program
run: $(PROGRAM)
	$(JAVAC) $(JFLAGS) $(OUT) $(CLASSES)

# Print the list of source files
print:
	echo "COMPILING: @$(JAVAC) $(JFLAGS) $(OUT) $(CLASSES)"

# Clean the build files
clean:
	echo "Cleaning..."
	rm -rf $(OUT)

.SUFFIXES: .java .class
.PHONY: all run print clean




