LD = mpiCC
CC = mpiCC

OPTFLAGS = -Wall 

SRCLIST = ParticleFactory.cpp main.cpp

EXEC = particles.x

SRCDIR = .
OBJDIR = obj

SRC = $(SRCLIST:%=$(SRCDIR)/%)

OBJ = $(filter %.o,$(SRCLIST:%.cpp=$(OBJDIR)/%.o))

all: greet $(EXEC)

greet:
	@echo "Compiling module" $(EXEC) "in" `pwd`

$(EXEC): $(OBJDIR) $(OBJ)
	$(LD) $(OPTFLAGS) $(OBJ) -o $@

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $<

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	-rm -f $(OBJDIR)/*.o 

clean_exec: 
	-rm -f $(EXEC)

purge: clean clean_exec

debug: all


