// TODO: Aumentare altezza dei bottoni

#include"tier1Browser.h"

// base
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// ROOT
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"

// GERDA
#include "MGTWaveform.hh"

// constructor
Tier1EventBrowser::Tier1EventBrowser(const TGWindow *p , int w , int h , char *filename ) {

    // Retrieve file names
    std::vector<std::string> listOfFiles;
    std::ifstream filelist( filename );
    std::string name;
    while ( filelist >> name ) listOfFiles.push_back(name);

    // initialize tree
    fTree = new TChain("MGTree");
    
    // Retrieve tier1 files
    for ( unsigned int i = 3 ; i < listOfFiles.size()-2 ; i++ ) fTree->Add(listOfFiles.at(i).c_str());

    fTotEvents = fTree->GetEntries();
    
    // set branch address
    fTree->SetBranchAddress( "event" , &fEvent );
     
    // Retrieve fSelectionList file
    TFile * fileWithList = new TFile( listOfFiles.at(listOfFiles.size()-1).c_str() , "READ" );
    if ( fileWithList->IsZombie() ) { 
        std::cout << "File is zombie! Tier1EventBrowser::fSelectionList is nullptr..." << std::endl;
        fSelectionList = nullptr;
    }
     else fSelectionList = (TEventList*)fileWithList->Get("listofselected")->Clone();
    
    // create main frame
    fMain = new TGMainFrame( p , w , h );
    
    // create embedded canvas
    fEmbCanvas = new TRootEmbeddedCanvas( "Ecanvas" , fMain , 900 , 600 );
    fMain->AddFrame( fEmbCanvas , new TGLayoutHints( kLHintsExpandX | kLHintsExpandY , 10 , 10 , 10 , 1 ));
    
    // create subframe for buttons
    fButtonsFrame = new TGHorizontalFrame( fMain , 600 , 10 , kChildFrame | kSunkenFrame );
    
    // buttons
    fPrev = new TGTextButton( fButtonsFrame , "&Prev" );
    fNext = new TGTextButton( fButtonsFrame , "&Next" );
    fNumberEntry = new TGNumberEntryField( fButtonsFrame , -1 , 0 , TGNumberFormat::kNESInteger , TGNumberFormat::kNEANonNegative , TGNumberFormat::kNELLimitMinMax , 0 , fTotEvents-1 );
    fExit = new TGTextButton( fButtonsFrame , "&Exit" , "gApplication->Terminate(0)" );
    
    // attach buttons to subframe
    fButtonsFrame->AddFrame( fPrev        , new TGLayoutHints( kLHintsCenterX | kLHintsExpandX , 5 , 5 , 3 , 4 ));
    fButtonsFrame->AddFrame( fNext        , new TGLayoutHints( kLHintsCenterX | kLHintsExpandX , 5 , 5 , 3 , 4 ));
    fButtonsFrame->AddFrame( fNumberEntry , new TGLayoutHints( kLHintsLeft , 5 , 5 , 3 , 4 ));
    fButtonsFrame->AddFrame( fExit        , new TGLayoutHints( kLHintsLeft , 5 , 5 , 3 , 4 ));
    
    // attach subframe to mainframe
    fMain->AddFrame( fButtonsFrame , new TGLayoutHints( kLHintsCenterX | kLHintsExpandX , 2 , 2 , 2 , 2 ));

    fPrev->Connect( "Clicked()" , "Tier1EventBrowser" , this , "DecrementAndDraw()" );
    fNext->Connect( "Clicked()" , "Tier1EventBrowser" , this , "IncrementAndDraw()" );
    fNumberEntry->Connect( "ReturnPressed()" , "Tier1EventBrowser" , this , "RetrieveAndDraw()" );

    fMain->SetWindowName( "GERDA tier1 fEvent browser" );
    fMain->MapSubwindows();
    fMain->Resize( fMain->GetDefaultSize() );
    fMain->MapWindow();   

    // draw something
    fCurrEvent = 0;
    DoDraw(fCurrEvent);
}

// destructor
Tier1EventBrowser::~Tier1EventBrowser() {
    // delete all graphic objects
    Cleanup();
    delete fMain;
    // delete the rest
    delete fTree;
    delete fEvent;
    delete fSelectionList;
}

void Tier1EventBrowser::DoDraw( long ev ) {
    
    if ( ev > fTotEvents-1 || ev < 0 ) return;

    TCanvas *can = fEmbCanvas->GetCanvas();
    can->cd();
    fEvent = new MGTEvent();
    fTree->GetEntry(ev);
    TH1D *fHist = fEvent->GetWaveform(0)->GimmeHist();
    fHist->SetLineColor(kBlue);
    if ( fSelectionList->Contains(ev) ) fHist->SetLineColor(kRed);
    
    // set title
    fHist->SetStats(kFALSE);
    std::ostringstream convert;
    convert.str("");
    convert << ev;
    std::string title = "Event " + convert.str();
    fHist->SetTitle( title.c_str() );   

    fHist->Draw();
    can->Update();

    fCurrEvent = ev;

    return;
}

void Tier1EventBrowser::IncrementAndDraw() { if ( fCurrEvent < fTotEvents-1 ) { fCurrEvent++; Tier1EventBrowser::DoDraw(fCurrEvent); } }

void Tier1EventBrowser::DecrementAndDraw() { if ( fCurrEvent > 0 ) { fCurrEvent--; Tier1EventBrowser::DoDraw(fCurrEvent); } }

void Tier1EventBrowser::RetrieveAndDraw() {  DoDraw(Tier1EventBrowser::fNumberEntry->GetIntNumber()); }

///////////////////////////////// main ////////////////////////////////////////////////////
#include "TApplication.h"

using namespace std;
int main( int argc , char** argv ) {
    
    // Help message
	if ( argc == 2 && strcmp( argv[1] , "--help" ) == 0 ) {
        cout << endl
             << "Event browser for tier1 data tree."                  << endl
		     << endl
             << "Usage:   analysis [filelist] [OPTIONS]"              << endl
		     << "Options: --help: display this help message and fExit" << endl
             << endl;
		return 0;
	} 
        
    // initialize ROOT dictionaries
    TApplication app( "app" , &argc , argv );  
  
    // create a Tier1EventBrowser object
    Tier1EventBrowser * myBrowser = new Tier1EventBrowser( gClient->GetRoot() , 900 , 600 , app.Argv()[1] );

    // run application
    app.Run();

    return 0;
}
