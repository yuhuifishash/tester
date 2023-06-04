CC=g++ -std=c++11 -g -Wall
CFLAGS = -I./include
#需要链接的库
LFLAGS = 
LDEXEFLAGS = 
OBJDIR = obj
SRCDIR += src

ALL_SRC += $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.cpp))
ALL_OBJ += $(patsubst src/%.cpp, $(OBJDIR)/%.o, $(ALL_SRC))

all:tester

tester: $(ALL_OBJ)
	$(CC) $(LFLAGS) $(ALL_OBJ) $(LDEXEFLAGS) -o bin/$@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) $(ALL_INCLUDE) -c $< -o $@

clean:
	rm -f ./obj/*.o server
