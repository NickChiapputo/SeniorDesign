PDF = pdflatex
OUT_FILE = Proposal/Proposal
SRC = Proposal/proposal.tex

pdf: clean $(SRC)
	pdflatex -halt-on-error -jobname=$(OUT_FILE) $(SRC) && pdflatex -halt-on-error -jobname=$(OUT_FILE) $(SRC) && make clean

clean:
	rm -f Proposal/*.aux Proposal/*.log 
