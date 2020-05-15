SRC=./src
PROJ=projektcpp
ARCHIVE=xlogin00-xsovam00
EXAMPLES=./examples
DOC=./doc

make: qmake
	make --directory=$(SRC)

pack: clean-all
	zip -r $(ARCHIVE).zip $(SRC) Makefile $(EXAMPLES) $(DOC)

clean-all: clean
	rm -rf $(DOC)/*

clean: qmake
	make --directory=$(SRC) clean
	rm -f $(SRC)/Makefile $(SRC)/$(PROJ) $(SRC)/$(PROJ).pro.user*

run: qmake
	make --directory=$(SRC)
	cd $(SRC) && ./$(PROJ)
qmake:
	qmake-qt5 $(SRC)/$(PROJ).pro -o $(SRC)/Makefile
