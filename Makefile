EXE = wordpairs
CC = gcc
OBJDIR = obj
GET_WORD = hw1/getWord/include
SRC = $(wildcard *.c) $(wildcard $(GET_WORD)/*.c)
HDR = $(wildcard *.h) $(wildcard $(GET_WORD)/*.h) 
OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

LIBS = -lm -Wall

$(EXE): $(OBJ)
	$(CC) $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(SRC))) -o $(EXE) $(LIBS)

$(OBJDIR)/%.o: %.c $(HDR) | $(OBJDIR)
	$(CC) -c $< -o  $(OBJDIR)/$(notdir $@) $(LIBS)

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)


debug: $(SRC) $(HDR)
	$(CC) $(SRC) -g -o $(EXE) $(LIBS)

fast: $(SRC) $(HDR)
	$(CC) $(SRC) -g -o $(EXE) $(LIBS) -O3
 

