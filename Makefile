EXE = wordpairs
CC = gcc
OBJDIR = obj
GET_WORD = ./hw1/getWord/include
WORDC = $(wildcard GET_WORD/*.c)
WORDH = $(wildcard GET_WORD/*.h)
SRC = $(wildcard *.c) 
HDR = $(wildcard *.h) 
OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))
WORDOBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(WORDC))
LIBS = -lm -Wall

$(EXE): $(OBJ) $(WORDOBJ)
	$(CC) $(OBJ) -o $(EXE) $(LIBS)

$(OBJDIR)/%.o: %.c $(HDR) $(WORDH) | $(OBJDIR)
	$(CC) -c $< -o $@ $(LIBS)

$(OBJDIR):
	mkdir -p $@

clean:
	rm -rf $(OBJDIR)

debug: $(SRC) $(HDR) $(WORDC) $(WORDH)
	$(CC) $(SRC) -g -o $(EXE) $(LIBS)
