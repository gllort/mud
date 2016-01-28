
SRC=contrib.c db.c sockets.c develop.c matrix.c math.c parser.c 
OBJ=contrib.$(O) db.$(O) sockets.$(O) develop.$(O) matrix.$(O) math.$(O) parser.$(O) 

OBJ=$(addprefix $(OBJDIR)/,$(subst .c,.o,$(SRC)))

$(OBJDIR)/%.o: %.c
	$(CC) -I$(OBJDIR) -I.. $(CFLAGS) -o $@ -c $<

all: $(OBJ)
	ar rcu packages.$(A) $(OBJ)
	$(RANLIB) packages.$(A)

clean:
	rm -f *.$(O)
	rm -f packages.$(A)
