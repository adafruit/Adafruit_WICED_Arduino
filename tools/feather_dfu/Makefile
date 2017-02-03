CC     = gcc
LD		 = gcc
CFLAGS = -O0 -g3 -Wall -std=gnu99
LIBS   += -lusb-1.0
OBJDIR = _build

ifeq ($(OS),Windows_NT)
	LIBS   += -Llibusb-1.0
	BINFILE = feather_dfu.exe
	BINDIR  = win32
else
	BINFILE = feather_dfu
	
	OS := $(shell uname -s)
	ifeq ($(OS),Linux)
		BINDIR = linux
	endif
	ifeq ($(OS),Darwin)
		BINDIR  = osx
  endif 
endif

all: clean $(OBJDIR) feather_dfu

.PHONY: $(OBJDIR)
$(OBJDIR):
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)
	
$(OBJDIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

feather_dfu: $(OBJDIR)/feather_dfu.o
	$(LD) -o $(BINDIR)/$(BINFILE) $^ $(CFLAGS) $(LIBS)
  
clean: 
	@rm -rf $(OBJDIR)
	@rm -f $(BINDIR)/$(BINFILE)
