
#***************************************************#
#                                                   #
#    GNU Makefile to build libdxalloc.so library    #
#                                                   #
#***************************************************#

MAJOR   := 1
MINOR   := 0
PATCH   := 0
VERSION := $(MAJOR).$(MINOR)

PKGNAME := $(shell grep PACKAGE_BINARY include/version.h | cut -d '"' -f 2)
LIBNAME := lib$(PKGNAME)
LIBAR   := $(LIBNAME).a
LIBSO   := $(LIBNAME).so
LIBSOM  := $(LIBSO).$(MAJOR)
LIBSOV  := $(LIBSO).$(VERSION)

CC      ?=  gcc
CFLAGS  := -g -O3 -MMD $(shell pkg-config --cflags glib-2.0)
INCLUDE += -I./src

WFLAGS  := -Wall -Wextra -Wno-comment -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-function
WFLAGS  += -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Wformat-security -Wnull-dereference -Wstack-protector \
           -Wtrampolines -Walloca -Wvla -Warray-bounds=2 -Wimplicit-fallthrough=3 -Wshift-overflow=2 -Wcast-qual \
           -Wstringop-overflow=4 -Wconversion -Wint-conversion -Wlogical-op -Wduplicated-cond -Wduplicated-branches \
           -Wformat-signedness -Wshadow -Wstrict-overflow=4 -Wundef -Wswitch-default -Wswitch-enum -Wstack-usage=4096 -Wcast-align=strict

WFLAGS  += -fstack-protector-all -fstack-protector-strong -fstack-clash-protection #don't use -fPIE flag for sharedlibrary

LDFLAGS += -Wl,-Bsymbolic-functions -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,-z,separate-code $(shell pkg-config --libs glib-2.0)
LDLIBS  += -lm -ldl -rdynamic -lbacktrace

AR      ?= ar
ARFLAGS := rcs
RANLIB  ?= ranlib

ifneq (,$(wildcard $(HOME)/.termux))
    CPPFLAGS += -DTERMUX
    WFLAGS   += -Wno-deprecated-declarations
else
    CPPFLAGS += -Wdate-time -D_FORTIFY_SOURCE=2
endif

BOLD   := $(shell tput bold)
NC     := $(shell tput sgr0)

SRCDIR := src
OBJDIR := obj
LIBDIR := libs

# Source modules
SRC_ALLOC := $(SRCDIR)/alloc
SRC_TRACE := $(SRCDIR)/trace
SRC_LOGIT := $(SRCDIR)/logit

# Source files
SRCS := $(SRC_ALLOC)/dxalloc.c $(SRC_TRACE)/dxtrace.c $(SRC_LOGIT)/logit.c

# Object files
OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

# Build dependencies
DEPS := $(OBJS:%.o=%.d)

all: info sharedlibrary staticlibrary

info:
	@echo "\n$(BOLD)Building $(LIBNAME) v$(VERSION) ...$(NC)"

# Build static library
staticlibrary: $(OBJS)
	@mkdir -p $(LIBDIR)
	@echo "\n$(BOLD)Building static library ...$(NC)"
	$(AR) $(ARFLAGS) $(LIBDIR)/$(LIBAR) $(OBJS)
	$(RANLIB) $(LIBDIR)/$(LIBAR)

# Build shared library
CFLAGS  += -fPIC
LDFLAGS += -shared -Wl,-soname,$(LIBSOM)
sharedlibrary: $(OBJS)
	@mkdir -p $(LIBDIR)
	@echo "\n$(BOLD)Building shared library ...$(NC)"
	$(CC) -o $(LIBDIR)/$(LIBSOV) $(OBJS) $(LDFLAGS) $(LDLIBS)
	cd $(LIBDIR) &&                  \
	   ln -s -f $(LIBSOV) $(LIBSOM); \
	   ln -s -f $(LIBSOM) $(LIBSO);

# Build object file for each .c file
$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WFLAGS) $(INCLUDE) -c $< -o $@

PREFIX     ?= /usr/local
SYSLIBDIR  := $(PREFIX)/lib
SYSINCLUDE := $(PREFIX)/include
install:
	install -D -m 644 $(LIBDIR)/$(LIBAR)     $(DESTDIR)$(SYSLIBDIR)/$(LIBAR)
	install -D -m 775 $(LIBDIR)/$(LIBSOV)    $(DESTDIR)$(SYSLIBDIR)/$(LIBSOV)
	install -D -m 644 $(SRC_ALLOC)/dxalloc.h $(DESTDIR)$(SYSINCLUDE)/dxalloc.h
	cd $(DESTDIR)$(SYSLIBDIR)   && \
	   ln -sf $(LIBSOV) $(LIBSOM); \
	   ln -sf $(LIBSOM) $(LIBSO);
	@#ldconfig -n $(SYSLIBDIR)

uninstall:
	rm -f $(DESTDIR)$(SYSINCLUDE)/dxalloc.h
	rm -f $(DESTDIR)$(SYSLIBDIR)/$(LIBSO)*
	rm -f $(DESTDIR)$(SYSLIBDIR)/$(LIBAR)

COREFILES := core vgcore*
clean:
	rm -rf $(OBJDIR) $(LIBDIR)/$(LIBAR) $(LIBDIR)/$(LIBSO)* __ktags

-include $(DEPS)

.PHONY: all clean sharedlibrary staticlibrary info install uninstall

#EOF
