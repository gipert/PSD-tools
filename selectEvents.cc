#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>

//ROOT
#include<TFile.h>
#include<TH1D.h>
#include<TCanvas.h>
#include<TApplication.h>
#include<TEventList.h>
#include<TTree.h>
#include<TString.h>

//GERDA
#include<MGTEvent.hh>
#include<MGTWaveform.hh>

using namespace std;

int main( int argc , char** argv ) {

    // Help message
	if ( argc == 2 && strcmp( argv[1] , "--help" ) == 0 ) {
        cout << endl
             << "The program selects the good events in tier1 and stores"   << endl 
             << "them in a TEventList object saved in list.root."           << endl
		     << endl
             << "Usage:   selectEvents [filelist] [sugmaCut]"               << endl
		     << "Options: --help:       display this help message and exit" << endl
             << endl;
		return 0;
	}

	string sigmaCutStr = argc > 2 ? argv[2] : "20";
	int sigmaCut = stoi(sigmaCutStr);

    cout << "Reading the input..." << endl;
    
    // Retrieve file names
    vector<string> listOfFiles;
    ifstream filelist( argv[1] );
    string name;
    while ( filelist >> name ) listOfFiles.push_back(name);

    // Retrieve channel number
    stringstream ssch;
    ssch << listOfFiles.at(2);
    int ch;
    ssch >> ch;

    // Retrieve tier2 master tree
    TFile tier2File( listOfFiles.at(listOfFiles.size()-2).c_str() , "READ" );
    TTree * masterTree = (TTree*)tier2File.Get("tree")->Clone();
 
    // Check for baseline problems and for failures from GELATIO
    cout << "Check for baseline problems and for failures from GELATIO..." << endl;
    masterTree->Draw( ">>listofselected" , Form("(GEMDBaseline_1.restoredWfBaselineSigma[%i]<%i) && (GEMDQuality_1.isgood[%i] == 1)" , ch , sigmaCut , ch ) );
    // isGood = 1 means GOOD event
    TEventList * listOfSelected = (TEventList*)gDirectory->Get("listofselected");
    cout << "Done, selected: " << listOfSelected->GetN() << "/" << masterTree->GetEntries() << endl;

    // Save list
    TFile file2( "list.root" , "RECREATE" );
    file2.cd();
    listOfSelected->Write();
    file2.Close();

    return 0;
}
