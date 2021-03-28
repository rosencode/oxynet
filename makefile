# 'make' 			build executable 'oxynet'
# 'make clean'	    removes all .o and executable files

CC 			= g++
CXXFLAGS 	= -std=c++17 -Wall
OPTFLAGS 	= -O3

TARGET		= oxynet
SRCDIR 		= ./src
SRCS		= $(SRCDIR)/main.cpp $(SRCDIR)/Parameters.cpp $(SRCDIR)/Connections.cpp
OBJS 		= $(SRCS:.cpp=.o)
	
RM			= rm -f

.PHONY: clean

install: $(TARGET)
	@echo Succesfully build $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(CXXFLAGS) $(OPTFLAGS) -c $< -o $@	

clean:
	@echo "Cleaning up.."
	$(RM) $(SRCDIR)/*.o $(SRCDIR)/*.d $(TARGET) 
