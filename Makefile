# MAKEFILE

CC         = c++

ROOTFLAGS  = $(shell root-config --cflags)
MGDOFLAGS  = $(shell mgdo-config --cflags)
CLHEPFLAGS = $(shell clhep-config --include)
ALLFLAGS   = $(ROOTFLAGS) $(MGDOFLAGS) $(CLHEPFLAGS)

ROOTLIBS  = $(shell root-config --glibs) -lSpectrum
MGDOLIBS  = $(shell mgdo-config --libs)
CLHEPLIBS = $(shell clhep-config --libs)
ALLLIBS   = $(ROOTLIBS) $(MGDOLIBS) $(CLHEPLIBS)

EXEC = tier1browser selectEvents currentPlot

all : $(EXEC)

selectEvents : selectEvents.cc
	mkdir -p bin && \
	$(CC) $(ALLFLAGS) -o bin/$@ $< $(ALLLIBS)

currentPlot : currentPlot.cc
	mkdir -p bin && \
	$(CC) $(ROOTFLAGS) -o bin/$@ $< $(ROOTLIBS)

tier1browser : tier1Browser.cxx tier1Browser.h tier1BrowserDict.cxx
	mkdir -p bin && \
	$(CC)-4.9 $(ALLFLAGS) -I. -o bin/$@ $< lib/tier1BrowserDict.cxx $(ALLLIBS)   

tier1BrowserDict.cxx : tier1Browser.h tier1BrowserLinkDef.h
	mkdir -p lib && \
	cd lib && \
	rootcling -f $@ $(MGDOFLAGS) $(CLHEPFLAGS) -c ../tier1Browser.h ../tier1BrowserLinkDef.h; \
	cd ..

.PHONY : all clean

clean : 
	rm -rf bin lib
