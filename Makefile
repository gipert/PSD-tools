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

EXEC = bin/tier1browser bin/selectEvents bin/currentPlot

all : $(EXEC)

bin/selectEvents : selectEvents.cc
	mkdir -p bin && \
	$(CC) $(ALLFLAGS) -o $@ $< $(ALLLIBS)

bin/currentPlot : currentPlot.cc
	mkdir -p bin && \
	$(CC) $(ROOTFLAGS) -o $@ $< $(ROOTLIBS) -fopenmp -Ofast

bin/tier1browser : tier1Browser.cxx lib/tier1BrowserDict.cxx
	mkdir -p bin && \
	$(CC)-4.9 $(ALLFLAGS) -I. -o $@ $< lib/tier1BrowserDict.cxx $(ALLLIBS)   

lib/tier1BrowserDict.cxx : tier1Browser.h tier1BrowserLinkDef.h
	mkdir -p lib && \
	cd lib && \
	rootcling -f tier1BrowserDict.cxx $(MGDOFLAGS) $(CLHEPFLAGS) -c ../tier1Browser.h ../tier1BrowserLinkDef.h; \
	cd ..

.PHONY : all clean

clean : 
	rm -rf bin/* lib/*
