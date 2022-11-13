
#***********************************************#
#                                               #
#    GNU Makefile to build Dxalloc test.out     #
#                                               #
#***********************************************#

PKGNAME  :=  Dxtest
ELFNAME  :=  dxtest
TARGET   :=  $(ELFNAME).out

PREFIX   ?= /usr/local
SRCDIR   :=  src
OBJDIR   :=  obj
LIBDIR   :=  libs
SHARE    :=  share

CC       ?=  gcc
CFLAGS   := -g -O2 -MMD
INCLUDE  += -I../lib/src/alloc

WFLAGS   := -Wall -Wextra -Wno-comment -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-function
WFLAGS   += -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Wformat-security -Wnull-dereference -Wstack-protector \
            -Wtrampolines -Walloca -Wvla -Warray-bounds=2 -Wimplicit-fallthrough=3 -Wshift-overflow=2 -Wcast-qual \
            -Wstringop-overflow=4 -Wconversion -Wint-conversion -Wlogical-op -Wduplicated-cond -Wduplicated-branches \
            -Wformat-signedness -Wshadow -Wstrict-overflow=4 -Wundef -Wswitch-default -Wswitch-enum -Wstack-usage=4096 -Wcast-align=strict
WFLAGS   += -fstack-protector-all -fstack-protector-strong -fstack-clash-protection -fPIE

ASAN     += -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fno-omit-frame-pointer \
            -fsanitize=bounds-strict -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow \
            -fsanitize=undefined

LDFLAGS  += -Wl,-Bsymbolic-functions -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,-z,separate-code \
            -Wl,-rpath=$(DEST)$(PREFIX)/lib -L../lib/$(LIBDIR)
LDLIBS   += -ldxalloc -lbacktrace

ifneq (,$(wildcard $(HOME)/.termux))
    CPPFLAGS += -DTERMUX
    WFLAGS   += -Wno-deprecated-declarations
else
    #CFLAGS   += $(ASAN)
    #LDLIBS   += $(ASAN)
    CPPFLAGS += -Wdate-time -D_FORTIFY_SOURCE=2
endif

BOLD := $(shell tput bold)
NC   := $(shell tput sgr0)

# Dxalloc test sources
SRCS := $(SRCDIR)/usage.c

# Object files
OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

# Build dependencies
DEPS := $(OBJS:%.o=%.d)

# Makefile execution starts here
all: info $(TARGET)

info:
	@echo "\n$(BOLD)Building $(PKGNAME) ...$(NC)"

# Link object files and generate executable
$(TARGET): $(OBJS)
	@echo  "\n$(BOLD)Linking object files ...$(NC)"
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LDLIBS)
	@echo "$(PKGNAME) build completed [$(TARGET)]"

# Build object file for each .c file
$(OBJDIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(WFLAGS) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

SYSBINDIR := $(PREFIX)/bin
install:
	install -D $(TARGET) $(DESTDIR)$(SYSBINDIR)/$(ELFNAME)

uninstall:
	rm -f $(DESTDIR)$(SYSBINDIR)/$(ELFNAME)

COREFILES := core vgcore*
clean:
	rm -rf $(TARGET) $(OBJDIR) $(COREFILES) __ktags

-include $(DEPS)

.PHONY: all clean info install uninstall 

#EOMake
