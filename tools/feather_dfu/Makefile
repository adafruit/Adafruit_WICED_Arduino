CC     = gcc
LD		 = gcc
CFLAGS = -O0 -g3 -Wall -std=gnu99
LIBS   += -lusb-1.0
BINDIR = _build

ifeq ($(OS),Windows_NT)
BINFILE=feather_dfu.exe
LIBS += -L.
else
BINFILE=feather_dfu
endif

# -I. -L./mingw32
all: $(BINDIR) feather_dfu

.PHONY: $(BINDIR)
$(BINDIR):
	@mkdir -p $(BINDIR)
	
$(BINDIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

feather_dfu: $(BINDIR)/feather_dfu.o
	$(LD) -o $(BINFILE) $^ $(CFLAGS) $(LIBS)
	
#gcc -o feather_dfu.exe $^ $(CFLAGS) $(LIBS)
  
clean: 
	@rm -rf $(BINDIR)
	@rm $(BINFILE)
