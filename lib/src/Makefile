
# GNU makefile

PKGNAME   =  Testsuite
ELFNAME   =  test
TARGET    =  $(ELFNAME).out

PREFIX   ?= /usr/local
SRCDIR   :=  src
OBJDIR   :=  obj
LIBDIR   :=  libs

CC       ?=  gcc
CPPFLAGS := -Wdate-time -D_FORTIFY_SOURCE=2
CFLAGS   := -g -O3 -MMD $(shell pkg-config --cflags glib-2.0)
UWARNS   := -Wno-pedantic -Wtraditional-conversion -Wstrict-prototypes
WFLAGS   := -Wall -Wextra -Wno-comment -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-unused-function
WFLAGS   += -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Wformat-security -Wnull-dereference -Wstack-protector \
            -Wtrampolines -Walloca -Wvla -Warray-bounds=2 -Wimplicit-fallthrough=3 -Wshift-overflow=2 -Wcast-qual \
            -Wstringop-overflow=4 -Wconversion -Wint-conversion -Wlogical-op -Wduplicated-cond -Wduplicated-branches \
            -Wformat-signedness -Wshadow -Wstrict-overflow=4 -Wundef -Wswitch-default -Wswitch-enum -Wstack-usage=4096 -Wcast-align=strict
WFLAGS   += -fstack-protector-all -fstack-protector-strong -fstack-clash-protection -fPIE
INCLUDE  += -I./
LDFLAGS  += -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -Wl,-z,separate-code $(shell pkg-config --libs glib-2.0)
LDLIBS   += -lm -ldl -lbacktrace -rdynamic

ASAN     += -fsanitize=address -fsanitize=pointer-compare -fsanitize=pointer-subtract -fsanitize=leak -fno-omit-frame-pointer \
            -fsanitize=bounds-strict -fsanitize=float-divide-by-zero -fsanitize=float-cast-overflow \
            -fsanitize=undefined

CFLAGS  += $(ASAN)
LDLIBS  += $(ASAN)

# C sources
SRCS := ./test.c ./alloc/dxalloc.c ./trace/dxtrace.c ./logit/logit.c

# Object files
OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

# Build dependencies
DEPS := $(OBJS:%.o=%.d)

# Makefile execution starts here
all: ktags $(TARGET)

# Link object files and generate executable
$(TARGET): $(OBJS)
	@echo  "\nLinking object files ..."
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) $(LDLIBS)
	@echo "$(PKGNAME) build completed [$(TARGET)]"

# Build object file for each .c file
$(OBJDIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WFLAGS) $(INCLUDE) -c $< -o $@

COREFILES := core vgcore*
clean:
	rm -rf $(TARGET) $(OBJDIR) $(COREFILES) __ktags

ktags:
	@ktags

-include $(DEPS)

.PHONY: all clean install uninstall

#EOMake

