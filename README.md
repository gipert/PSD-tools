Pulse Shape Analysis tools for GERDA's Germanium Detectors
==========================================================

Contents:
--------

The package provides three executables: 

* `tier1browser` is a basic waveform browser with a GUI (ROOT libraries) which
allows also to mark events included in a `TEventList` object provided by the user. Useful to verify the efficiency of a
selection filter.
* `selectEvents` helps to select tier1 events (selection stored into a `TEventList` object) with data coming from the
  GELATIO analysis.
* `currentPlot` computes the efficiency in neglecting the MSE events in the 212Bi peak at 1620.7 keV setting a cut on the near 208Tl
  (double photon escape) peak at 1592.5 keV.
  and provides a fancy output.

Instructions to use the tools can be retriven with the `--help` option. the `filelist` file has to be formatted in the
following manner:

    0.3456  -0.7865 1   // parameters for the energy calibration (m,q) and channel where data are stored
    tier1_data_0.root   // tier1 data files
    tier1_data_1.root   // ...
    ...
    tier1_data_n.root
    tier2_data.root     // tier2 data file
    list.root           // ROOT file with the TEventList object

The package also provides two ROOT's macros based on the `TSpectrum` class:

* `peakAnalysis.C` is a peak finding algorithm designed for this specific energy spectrum that produces a `calib.txt`
  ready to be used for energy calibration.
* `checkCalib.C` computes and removes the background from the calibrated spectrum as well as the resolution parameters
  of the 208Tl peak at 2614.5 keV.

Installation:
------------

Simply run

    make

to produce all the executables. You will need the MGDO, CLHEP and  ROOT libraries. Tested on Arch Linux with gcc v6.2.1
and v4.9. Take a look to the Makefile and modify it according to your setup.
