# MAKEFILE

CC         = gcc

ROOTFLAGS  = $(shell root-config --cflags)
MGDOFLAGS  = $(shell mgdo-config --cflags)
CLHEPFLAGS = $(shell clhep-config --include)

ROOTLIBS  = $(shell root-config --glibs) -lSpectrum
MGDOLIBS  = $(shell mgdo-config --libs)
CLHEPLIBS = $(shell clhep-config --libs)

OBJECTS = $(wildcard *.o)

all : tier1browser selectEvents currentPlot

selectEvents : selectEvents.o
	$(CC) -o selectEvents selectEvents.o $(ROOTLIBS) $(MGDOLIBS) $(CLHEPLIBS)

currentPlot : currentPlot.o
	$(CC) -o currentPlot currentPlot.o $(ROOTLIBS) $(MGDOLIBS) $(CLHEPLIBS)

tier1browser : tier1Browser.o tier1BrowserDict.o
	$(CC) -o tier1browser tier1Browser.o tier1BrowserDict.o $(ROOTLIBS) $(MGDOLIBS) $(CLHEPLIBS)   

selectEvents.o : selectEvents.cc
	$(CC) -c selectEvents.cc $(ROOTFLAGS) $(MGDOFLAGS) $(CLHEPFLAGS) 

currentPlot.o : currentPlot.cc
	$(CC) -c currentPlot.cc $(ROOTFLAGS) $(MGDOFLAGS) $(CLHEPFLAGS)

tier1Browser.o : tier1Browser.cxx tier1Browser.h
	$(CC) -c tier1Browser.cxx $(ROOTFLAGS) $(MGDOFLAGS) $(CLHEPFLAGS) -I.

tier1BrowserDict.o : tier1BrowserDict.cxx tier1Browser.h
	$(CC) -c tier1BrowserDict.cxx $(ROOTFLAGS) $(MGDOFLAGS) $(CLHEPFLAGS) -I.

tier1BrowserDict.cxx : tier1Browser.h tier1BrowserLinkDef.h
	rootcling -f tier1BrowserDict.cxx $(MGDOFLAGS) $(CLHEPFLAGS) -c tier1Browser.h tier1BrowserLinkDef.h 

.PHONY : clean

clean : 
	rm -f tier1browser selectEvents currentPlot tier1BrowserDict.cxx tier1BrowserDict_rdict.pcm $(OBJECTS)

#
#rm -f $FILELOC/loopOverList $FILELOC/currentPlot $FILELOC/tier1Browser
#
#rootcling -f $FILELOC/tier1BrowserDict.cxx `mgdo-config --cflags` `clhep-config --include` -c $FILELOC/tier1Browser.h $FILELOC/tier1BrowserLinkDef.h

#c++-4.9  -I. `root-config --cflags` `clhep-config --include` `mgdo-config --cflags` $FILELOC/tier1Browser.cxx $FILELOC/tier1BrowserDict.cxx -o $FILELOC/tier1browser `root-config --glibs` -lSpectrum `clhep-config --libs` `mgdo-config --libs`
#
#c++ -Wall -I. `root-config --cflags` `clhep-config --include` `mgdo-config --cflags` $FILELOC/selectEvents.cc -o $FILELOC/selectEvents `root-config --libs` -lSpectrum `clhep-config --libs` `mgdo-config --libs`
#
#c++ -Wall -I. `root-config --cflags` `clhep-config --include` `mgdo-config --cflags` $FILELOC/currentPlot.cc -o $FILELOC/currentPlot `root-config --libs` -lSpectrum `clhep-config --libs` `mgdo-config --libs`
