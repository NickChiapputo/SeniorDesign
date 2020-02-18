PDF = pdflatex
# OUT_FILE = Proposal$(shell date +'_%m.%d')
OUT_FILE = Proposal
SRC = proposal.tex

pdf: clean $(SRC)
	pdflatex -halt-on-error -jobname=$(OUT_FILE) $(SRC) && pdflatex -halt-on-error -jobname=$(OUT_FILE) $(SRC) && make clean

clean:
	rm -f *.aux *.log 
