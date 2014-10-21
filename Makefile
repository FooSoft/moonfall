TARGET = Moonfall

OBJDIRBASE = obj
DEPDIRBASE = dep
TARGETDIRBASE = bin

CPP = g++
ECHO = echo
RM = rm
MKDIR = mkdir
MAKE = make
EXIT = exit
LS = ls

CPPFILES = $(shell $(LS) -t *.cpp)

CPPFLAGS = -fno-strict-aliasing -Wno-multichar -Wall `wx-config --cflags` `sdl-config --cflags`
LFLAGS = `wx-config --libs` `sdl-config --libs` -llua -lSDL_image -lSDL_ttf

DBGCPPFLAGS = -g -pg -DDEBUG
RELCPPFLAGS =
DEVCPPFLAGS = -DDEVELOPER

DBGLFLAGS = -pg
RELLFLAGS =
DEVLFLAGS =

ifeq ($(CFG), LINUX_DBG_DEV)
CPPFLAGS += $(DBGCPPFLAGS)
CPPFLAGS += $(DEVCPPFLAGS)
LFLAGS += $(DBGLFLAGS)
LFLAGS += $(DEVLFLAGS)
CFGSUBDIR = LinuxDbgDev
endif
ifeq ($(CFG), LINUX_REL_DEV)
CPPFLAGS += $(RELCPPFLAGS)
CPPFLAGS += $(DEVCPPFLAGS)
LFLAGS += $(RELLFLAGS)
LFLAGS += $(DEVLFLAGS)
CFGSUBDIR = LinuxRelDev
endif
ifeq ($(CFG), LINUX_DBG)
CPPFLAGS += $(DBGCPPFLAGS)
LFLAGS += $(DBGLFLAGS)
CFGSUBDIR = LinuxDbg
endif
ifeq ($(CFG), LINUX_REL)
CPPFLAGS += $(RELCPPFLAGS)
LFLAGS += $(RELLFLAGS)
CFGSUBDIR = LinuxRel
endif

OBJDIR = $(OBJDIRBASE)/$(CFGSUBDIR)
DEPDIR = $(DEPDIRBASE)/$(CFGSUBDIR)
TARGETDIR = $(TARGETDIRBASE)/$(CFGSUBDIR)

TARGETFILE = $(TARGETDIR)/$(TARGET)
OBJFILES = $(CPPFILES:%.cpp=$(OBJDIR)/%.o)
DEPFILES = $(CPPFILES:%.cpp=$(DEPDIR)/%.d)

all: cfgcheck $(TARGETFILE)

$(TARGETFILE): dirs $(OBJFILES)
	@$(ECHO) "Linking" $(TARGETFILE)...
	$(CPP) $(LFLAGS) $(OBJFILES) -o $(TARGETFILE)

$(OBJDIR)/%.o: %.cpp
	@$(MKDIR) -p $(OBJDIR) $(DEPDIR)
	@$(ECHO) "Compiling" $*.cpp...
	@$(CPP) $(CPPFLAGS) -c $*.cpp -o $(OBJDIR)/$*.o
	@$(CPP) $(CPPFLAGS) -MM $*.cpp >> $(DEPDIR)/$*.d
	@$(ECHO) -n $(OBJDIR)"/" > $(DEPDIR)/$*.d ; $(CPP) $(CPPFLAGS) -MM $*.cpp >> $(DEPDIR)/$*.d

cfgcheck:
ifeq ($(CFGSUBDIR),)
	@$(ECHO) "Invalid build configuration (CFG); valid options are:"
	@$(ECHO) "	* LINUX_DBG_DEV"
	@$(ECHO) "	* LINUX_REL_DEV"
	@$(ECHO) "	* LINUX_DBG"
	@$(ECHO) "	* LINUX_REL"
	@$(EXIT) 1
endif

dirs:
	@$(MKDIR) -p $(OBJDIR) $(DEPDIR) $(TARGETDIR)

distclean: cleanall

clean: cfgcheck
	@$(ECHO) "Cleaning:" $(TARGET)...
	@$(RM) -f -r $(OBJDIR) $(DEPDIR) $(TARGETDIR)

cleanall:
	@$(ECHO) "Cleaning all..."
	@$(RM) -f -r $(OBJDIRBASE) $(DEPDIRBASE) $(TARGETDIRBASE)

ifeq (,$(findstring clean,$(MAKECMDGOALS)))
ifeq (,$(findstring cleanall,$(MAKECMDGOALS)))
ifeq (,$(findstring distclean,$(MAKECMDGOALS)))
-include $(DEPFILES)
endif
endif
endif
