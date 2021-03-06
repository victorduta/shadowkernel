
include $(ROOT)/Makefile.inc
HEADERS = $(wildcard $(INCLUDE_PASS_DIR)/$(PASSNAME)/*.h)
IFLAGS +=  -I$(INCLUDE_PASS_DIR)/$(PASSNAME) -I$(INCLUDE_PASS_DIR)

PASSLIBNAME =$(PASSNAME).so

HEADERS += $(wildcard $(LLVMINC)/*.h)


CFLAGS  += $(CXXFLAGS) -Wno-long-long -Wall -W -Wno-unused-parameter -Wwrite-strings -DHAVE_EXCEPTIONS=0
CFLAGS  += $(shell $(LLVMPREFIX)/bin/llvm-config --cxxflags) -DLLVM_VERSION=$(LLVM_VERSION) 
LDFLAGS += $(shell $(LLVMPREFIX)/bin/llvm-config --ldflags) 

INSTALL_DIR=${INSTALL_PASS_DIR}

CXX=c++
all: $(PASSLIBNAME)


$(PASSLIBNAME): $(OBJS)
	$(QUIET) $(ECHO) "  [LINK] $@"
	$(QUIET) $(CXX) $(CFLAGS) -shared -o $@ $(CPPS) $(OBJS) $(LDFLAGS) $(LIBS) 

%.o: %.cpp $(HEADERS)
	$(QUIET) $(ECHO)  "  [C++] $<"	
	$(QUIET) $(CXX) $(CFLAGS) $(IFLAGS) $(DFLAGS) -c -o $@ $< 

install: $(INSTALL_DIR)/$(PASSLIBNAME)

$(INSTALL_DIR)/$(PASSLIBNAME): $(PASSLIBNAME)
	$(QUIET) $(ECHO) "  [INSTALL] $< -> $@"
	$(QUIET) install -c -D -m 744 $? $@

clean:
	$(QUIET) $(ECHO) "  [RM] $(OBJS) $(PASSLIBNAME)"
	$(QUIET) rm -f $(OBJS) $(PASSLIBNAME)

distclean: clean
	$(QUIET) $(ECHO)  "  [RM] $(INSTALL_DIR)/$(PASSLIBNAME)"
	$(QUIET) rm -f $(INSTALL_DIR)/$(PASSLIBNAME)
