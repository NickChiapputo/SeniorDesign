PDF = pdflatex
# OUT_FILE = Proposal$(shell date +'_%m.%d')
OUT_FILE = ATIS_Abstract
SRC = abstract.tex

pdf: clean $(SRC)
	pdflatex -halt-on-error -jobname=$(OUT_FILE) $(SRC) > /dev/null && pdflatex -halt-on-error -jobname=$(OUT_FILE) $(SRC) && make clean

clean:
	rm -f *.aux *.log 
