# Define the machine object files for your program
OBJECTS = abc123p3.o BrowserList.o DoublyLinkedList.o
# Define your include file
INCLUDES = BrowserList.h DoublyLinkedList.h

# make for the executable
p3: ${OBJECTS}
	gcc -g -o p3 ${OBJECTS}

# Simple suffix rules for the .o
%.o: %.c ${INCLUDES}
	gcc -g -c $<

# Clean the .o files
clean:
	rm -f ${OBJECTS}
