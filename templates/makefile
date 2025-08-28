# ======================================================================================
# USER-CONFIGURABLE SETTINGS
# ======================================================================================

# name of program without file ending that is to be compiled
BINARY := 

# Python script to be executed
PYTHONSCRIPT := 

# LaTeX script to be executed
LATEXSCRIPT := 

# header files (in case you want to link own lib)
HEADER_NUM := 
HEADER_PRO := 

# relative path to header files
HEADER_PATH := 

# set to 1 of you used those libraries
USE_LAPACKE := 0
# (not necessary if you link tlib which already contains GSL)
USE_GSL := 0

# set to 1 if you use external libraries
# then set include-, lib-path and lib-name in dependencies.txt or your prefered name 
# of file using follwing pattern:
# includepath/header.h
# # without file ending
# libpath/lib
# name:libname
USE_EXT_LIBS := 0

# file is not cleaned
DEPENDENCY_FILE := dependencies.txt

# arguments to be passed to program
ARGS :=

# optionally set to 1 to add Homebrew include and lib paths generally
USE_HOMEBREW := 0

# optionally set to 1 to allow nano zone
# this is a part of heap, usually several hundred kB, which is especially fast
# in allocation for small structs. But it can lead to issues in combination with other
# flags # (mainly libraries). Therefore, default is 0
USE_NANO := 0

# ======================================================================================
# COMPILER SETTINGS
# ======================================================================================

# used C compiler
CC := gcc

# warning flags
WFLAGS := -Wall -Wextra -Wshadow -pedantic -fstack-protector

# link math
MFLAGS := -lm

# Optimization flags --> O1, O2, O3, Ofast
OFLAGS := -O3
# Link-time optimization flags
LFLAGS := -flto

# Debugging flags
DFLAGS := -g -fsanitize=address

# ======================================================================================
# AUTOMATED INCLUDE AND LINKING PATHS
# ======================================================================================

# --------------------------------------
# INITIALIZATION OF LIBRARY FLAGS
# --------------------------------------

# compilation flags for libraries
CLIBS :=
# linking flags for libraries
LLIBS := $(MFLAGS)

# --------------------------------------
# GSL
# --------------------------------------

# GSL linking flags
GSLLFLAGS := -lgsl -lgslcblas

# Auto-detected GSL linking flags and paths via pkg-config
GSL_CFLAGS := $(shell pkg-config --cflags gsl 2>/dev/null)
GSL_LIBS   := $(shell pkg-config --libs gsl 2>/dev/null)

# fallback GSL flags if pkg-config is not available --> GSL_LIBS == NONE
ifeq ($(strip $(GSL_LIBS)),)
GSL_LIBS := $(GSLLFLAGS)
endif

# --------------------------------------
# LAPACKE
# --------------------------------------

# Lapacke link flags
LPFLAGS := -llapacke -llapack -lblas

# Auto-detected LAPACKE linking flags and paths via pkg-config
LAPACKE_CFLAGS := $(shell pkg-config --cflags lapacke 2>/dev/null)
LAPACKE_LIBS   := $(shell pkg-config --libs lapacke 2>/dev/null)

# fallback
ifeq ($(strip $(LAPACKE_LIBS)),)
LAPACKE_LIBS := $(LPFLAGS)
endif

# --------------------------------------
# INCLUSION IF USED
# --------------------------------------

# Conditionally include LAPACKE if used
ifeq ($(USE_LAPACKE),1)
CLIBS += $(LAPACKE_CFLAGS)
LLIBS += $(LAPACKE_LIBS)
endif

# Conditionally include GSL if used
ifeq ($(USE_GSL),1)
CLIBS += $(GSL_CFLAGS)
LLIBS += $(GSL_LIBS)
endif

# --------------------------------------
# ADDITION OF HOMEBREW PATHS
# --------------------------------------

ifeq ($(USE_HOMEBREW),1)
# this time we do not use 2>/dev/null
# since if user decides to use Homebrew
# this shell command should not fail
BREW_PREFIX := $(shell brew --prefix)
CLIBS += -I$(BREW_PREFIX)/include
LLIBS   += -L$(BREW_PREFIX)/lib
endif

# --------------------------------------
# ADDITION OF EXTERNAL LIBS
# --------------------------------------

# this sections works for multiple external libraries
# in dependencies.txt
ifeq ($(USE_EXT_LIBS),1)
# throw error if dependencies.txt is missing
ifeq ($(wildcard $(DEPENDENCY_FILE)),)
$(error Dependency file $(DEPENDENCY_FILE) not found!)
endif
# read dependencies.txt and extract paths
DEPS_PATHS := $(shell cat $(DEPENDENCY_FILE))

# strip whitespaces
DEPS_PATHS := $(strip $(DEPS_PATHS))

# extract include paths (everything with "include" in the name)
INCLUDE_DIRS := $(sort $(dir $(filter %include/%,$(DEPS_PATHS))))

# extract library paths (everything with "lib" in the name)
LIB_DIRS := $(filter %/lib,$(DEPS_PATHS))

# extract names of linked libraries
LIB_NAMES := $(patsubst name:%,%,$(filter name:%,$(DEPS_PATHS)))

# add include-, lib-path and linking flags
CLIBS += $(addprefix -I,$(INCLUDE_DIRS))
LLIBS += $(addprefix -L,$(LIB_DIRS))
LLIBS += $(addprefix -l,$(LIB_NAMES))
endif

# always print recognized paths and libraries
$(info LIB_DIRS=$(LIB_DIRS))
$(info LIB_NAMES=$(LIB_NAMES))

# --------------------------------------
# FINAL FLAGS ASSEMBLY
# --------------------------------------

CFLAGS := $(WFLAGS) $(OFLAGS) $(LFLAGS)
DEBUGFLAGS := $(WFLAGS) $(DFLAGS) $(LFLAGS)

# ======================================================================================
# BUILD RULES
# ======================================================================================

# pattern rule for object files:
# -MMP -MP generates dependency files (.d) so that compilation restarts if header files
# were modified,
# automatic variable $< calls first file from dependencies of target, usually .c file
# $@ calls the target file name, usually .o file

# optionally write path before *.c and *.o if you want to link object files from 
# another path
SRC := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRC))

%.o: %.c
	$(CC) -MMD -MP -c $< -o $@ $(CLIBS) $(CFLAGS)

# include header dependency files
-include *.d

$(BINARY): $(BINARY).o
	$(CC) -o $@ $^ $(CFLAGS) $(LLIBS)

debugging: $(BINARY).c
	$(CC) -o $(BINARY) $(OBJS) $(BINARY).c $(DEBUGFLAGS) $(LLIBS)


# ======================================================================================
# PROGRAM EXECUTION
# ======================================================================================

# declare phony targets
.PHONY: run plot tex clean

# targets if none is specified when calling make
all: run plot tex clean

# determine system name since DYLD_LIBRARY_PATH is for MacOS while LD_LIBRARY_PATH
# is for Linux
UNAME_S := $(shell uname -s)

run: $(BINARY)
##########################
ifeq ($(USE_EXT_LIBS),1)
#*************************
ifeq ($(USE_NANO),1)
#-------------------------
ifeq ($(UNAME_S),Darwin)
	MallocNanoZone=1 env DYLD_LIBRARY_PATH=$(LIB_DIRS) ./$(BINARY) $(ARGS)
#-------------------------
else
	MallocNanoZone=1 env LD_LIBRARY_PATH=$(LIB_DIRS) ./$(BINARY) $(ARGS)
#-------------------------
endif
#*************************
else
#-------------------------
ifeq ($(UNAME_S),Darwin)
	MallocNanoZone=0 env DYLD_LIBRARY_PATH=$(LIB_DIRS) ./$(BINARY) $(ARGS)
#-------------------------
else
	MallocNanoZone=0 env LD_LIBRARY_PATH=$(LIB_DIRS) ./$(BINARY) $(ARGS)
#-------------------------
endif
#*************************
endif
##########################
else
#*************************
ifeq ($(USE_NANO),1)
#-------------------------
ifeq ($(UNAME_S),Darwin)
	MallocNanoZone=1 ./$(BINARY) $(ARGS)
#-------------------------
else
	MallocNanoZone=1 ./$(BINARY) $(ARGS)
#-------------------------
endif
#*************************
else
#-------------------------
ifeq ($(UNAME_S),Darwin)
	MallocNanoZone=0 ./$(BINARY) $(ARGS)
#-------------------------
else
	MallocNanoZone=0 ./$(BINARY) $(ARGS)
#-------------------------
endif
#*************************
endif
##########################
endif

# --------------------------------------
# PYTHON SCRIPT EXECUTION
# --------------------------------------

plot:
	python3 $(PYTHONSCRIPT).py

# --------------------------------------
# LaTeX COMPILATION
# --------------------------------------

LATEXFLAGS :=
USE_MINTED := $(shell grep -q '\usepackage{minted}' $(LATEXSCRIPT).tex && echo 1 || echo 0)
ifeq ($(USE_MINTED),1)
LATEXFLAGS += -shell-escape
endif

tex:
	pdflatex $(LATEXFLAGS) $(LATEXSCRIPT).tex
	@if grep "biber" $(LATEXSCRIPT).tex; then \
		biber $(LATEXSCRIPT); \
	fi
	pdflatex $(LATEXFLAGS) $(LATEXSCRIPT).tex
	pdflatex $(LATEXFLAGS) $(LATEXSCRIPT).tex

# ======================================================================================
# CLEAN-UP RULES
# ======================================================================================

clean:
	@for f in *.txt; do \
		if [ "$$f" != "$(DEPENDENCY_FILE)" ]; then \
			rm -f "$$f"; \
		fi; \
	done
# executables and object files
	rm -f *.o *.exe $(BINARY)
# dependencies makefile
	rm -f *.d
# data formats
	rm -f *.dat *.csv
# images and documents
	rm -f *.png *.pdf *.md
# LaTeX auxiliary files
	rm -f *.aux *.log *.toc *.blg *.out *.bbl *.fdb_latexmk *.synctex.gz \
	      *.fls *.lof *.out.ps *.dvi *.lot *.bcf *.run *.run.xml

# ======================================================================================
# USAGE EXAMPLES
# ======================================================================================

# make                   → compile + plot + tex + clean
# make USE_HOMEBREW=1    → falls GSL über Homebrew installiert wurde
# make debugging         → Kompilation mit Debugflags
# make run ARGS="..."    → Ausführung mit Argumenten

# add:
# $(HEADER_NUM).o: $(HEADER_PATH)/$(HEADER_NUM).c
# 	$(CC) -MMD -MP -c $< -o $@ $(CLIBS) $(CFLAGS)

# $(HEADER_PRO).o: $(HEADER_PATH)/$(HEADER_PRO).c
# 	$(CC) -MMD -MP -c $< -o $@ $(CLIBS) $(CFLAGS)
# if you want to use your own header files

# In that case build recipes must me changed to
# $(BINARY): $(BINARY).o $(HEADER_NUM).o $(HEADER_PRO).o
# 	$(CC) -o $@ $^ $(CFLAGS) $(LLIBS) $(LDFLAGS)

# debugging: $(BINARY).c $(HEADER_NUM).o $(HEADER_PRO).o
# 	$(CC) -o $(BINARY) $(BINARY).c $(HEADER_NUM).o $(HEADER_PRO).o $(DEBUGFLAGS) $(LLIBS) $(LDFLAGS)
