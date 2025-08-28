# --------------------------------------
# SETTINGS
# --------------------------------------

# name
LATEXSCRIPT := main

# --------------------------------------
# LaTeX COMPILATION
# --------------------------------------

LATEXFLAGS :=
USE_MINTED := $(shell grep -q '\usepackage{minted}' $(LATEXSCRIPT).tex && echo 1 || echo 0)
ifeq ($(USE_MINTED),1)
    LATEXFLAGS += -shell-escape
endif

tex: $(LATEXSCRIPT).tex
	pdflatex $(LATEXFLAGS) $(LATEXSCRIPT).tex
	@if grep "biber" $(LATEXSCRIPT).tex; then \
		biber $(LATEXSCRIPT); \
	fi
	pdflatex $(LATEXFLAGS) $(LATEXSCRIPT).tex
	pdflatex $(LATEXFLAGS) $(LATEXSCRIPT).tex

clean:
	rm -f *.aux *.log *.toc *.blg *.out *.bbl *.fdb_latexmk *.synctex.gz \
	      *.fls *.lof *.out.ps *.dvi *.lot *.bcf *.run *.run.xml
# only here!!!	
	rm -f *.pdf