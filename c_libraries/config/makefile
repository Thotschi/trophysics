# color codes
MYCOLOR := \033[0;32m
NC := \033[0m # No Color

CC := gcc
USE_GSL := 1

WFLAGS := -Wall -Wextra -Wshadow -pedantic -fstack-protector
MFLAGS := -lm
OFLAGS := -O3
LFLAGS := -flto
LINUXFLAGS := -fPIC
DFLAGS := -g -fsanitize=address

GSLLFLAGS := -lgsl -lgslcblas
GSL_CFLAGS := $(shell pkg-config --cflags gsl 2>/dev/null)
GSL_LIBS   := $(shell pkg-config --libs gsl 2>/dev/null)

CLIBS :=
LLIBS := $(MFLAGS)

ifeq ($(USE_GSL),1)
CLIBS += $(GSL_CFLAGS)
LLIBS += $(GSL_LIBS)
endif

CFLAGS := $(WFLAGS) $(OFLAGS) $(LFLAGS)
DEBUGFLAGS := $(WFLAGS) $(DFLAGS) $(LFLAGS)

SRC := $(wildcard ../src/*.c)
OBJS := $(patsubst ../src/%.c, %.o, $(SRC))

build: $(OBJS)
	@echo "> $(MYCOLOR)Building shared library$(NC)..."
	$(CC) -shared -o libtlib.so $(OBJS) $(LLIBS) $(LINUXFLAGS)
	@echo "> $(MYCOLOR)Done$(NC)."

%.o: ../src/%.c
	$(CC) -MMD -MP -c $< -o $@ $(CLIBS) $(CFLAGS) $(LINUXFLAGS)

submit:
	@echo "> $(MYCOLOR)Deleting old library$(NC)..."
	rm -f ../lib/libtlib.so
	@echo "> $(MYCOLOR)Done$(NC)."
	@echo "> $(MYCOLOR)Moving shared lib to ../lib/$(NC)..."
	mv libtlib.so ../lib/
	@echo "> $(MYCOLOR)Done$(NC)."

-include *.d

clean:
	@echo "> $(MYCOLOR)Cleaning up...$(NC)"
# executables and object files
	rm -f *.o
# dependencies makefile
	rm -f *.d
	@echo "> $(MYCOLOR)Done$(NC)."

clean_so:
	@echo "> $(MYCOLOR)Cleaning shared library in ../lib/$(NC)..."
	rm -f ../lib/libtlib.so
	@echo "> $(MYCOLOR)Done$(NC)."