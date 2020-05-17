SRC=src
BUILD=build
PROJ=icp
ARCHIVE=xhalom00-xsovam00
EXAMPLES=examples
DOC=doc
QMAKE=qmake

make: qmake
	make --directory=$(BUILD)

pack: clean-all
	zip -r $(ARCHIVE).zip $(SRC) Makefile Doxyfile $(EXAMPLES) $(DOC)

clean-all: clean
	rm -rf $(DOC)/*

clean: qmake
	rm -rf $(BUILD)

run: qmake
	make --directory=$(BUILD)
	cd $(BUILD) && ./$(PROJ)

doxygen:
	doxygen Doxyfile

qmake:
	mkdir -p $(BUILD)
	$(QMAKE) $(SRC)/$(PROJ).pro -o $(BUILD)/Makefile 
