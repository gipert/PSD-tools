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
	$(CC) $(ALLFLAGS) -o $@ $< $(ALLLIBS)

currentPlot : currentPlot.cc
	$(CC) $(ROOTFLAGS) -o $@ $< $(ROOTLIBS)

tier1browser : tier1Browser.cxx tier1Browser.h tier1BrowserDict.cxx
	$(CC)-4.9 $(ALLFLAGS) -I. -o $@ $< tier1BrowserDict.cxx $(ALLLIBS)   

tier1BrowserDict.cxx : tier1Browser.h tier1BrowserLinkDef.h
	rootcling -f $@ $(MGDOFLAGS) $(CLHEPFLAGS) -c $^ 

.PHONY : clean

clean : 
	rm -rf $(EXEC) tier1BrowserDict.cxx tier1BrowserDict_rdict.pcm
