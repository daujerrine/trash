
INFILE = haskell.md

PANDOC = pandoc
PANDOC_FONT = "DejaVu Serif"
PANDOC_FLAGS := --standalone \
                --number-sections \
                --toc \
                --filter pandoc-numbering \
                --variable mainfont=$(PANDOC_FONT)

HTML_OUTFILE      := haskell_compiled.html

MARKDOWN_TEMPLATE := template.md
MARKDOWN_FLAGS    := --template $(MARKDOWN_TEMPLATE) -t markdown_strict
MARKDOWN_OUTFILE  := haskell_compiled.md

PDF_LATEX_HEADER := header.tex
PDF_FLAGS        := -H $(PDF_LATEX_HEADER) --pdf-engine=xelatex --listings
PDF_OUTFILE      := haskell_compiled.pdf


.PHONY: all clean pdf html md

all: $(HTML_OUTFILE) $(MARKDOWN_OUTFILE) $(PDF_OUTFILE)

pdf: $(PDF_OUTFILE)

html: $(HTML_OUTFILE)

md: $(MARKDOWN_OUTFILE)

clean:
	rm $(MARKDOWN_OUTFILE) $(HTML_OUTFILE) $(PDF_OUTFILE)

$(HTML_OUTFILE): $(INFILE)
	$(PANDOC) $< $(PANDOC_FLAGS) -t html -o $@

$(PDF_OUTFILE): $(INFILE)
	$(PANDOC) $< $(PANDOC_FLAGS) $(PDF_FLAGS) -o $@

$(MARKDOWN_OUTFILE): $(INFILE)
	$(PANDOC) $< $(PANDOC_FLAGS) $(MARKDOWN_FLAGS) -o $@
