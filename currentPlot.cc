#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include<omp.h>

// ROOT
#include<TFile.h>
#include<TTree.h>
#include<TH2D.h>
#include<TH1D.h>
#include<TGraph.h>
#include<TAxis.h>
#include<TCanvas.h>
#include<TPad.h>
#include<TApplication.h>
#include<TEventList.h>
#include<TLegend.h>
#include<TLine.h>
#include<TText.h>

using namespace std;

int main( int argc , char** argv ) {

    // Help message
	if ( argc == 2 && strcmp( argv[1] , "--help" ) == 0 ) {
        cout << endl
             << "Program to plot A/E vs. E from a tier2 file."                                          << endl
             << "\tRemember: - [yAxis_lenght] has to be adjusted to make the program running properly." << endl
             << "\t          - The binning used for drawing the two histograms on the right is not the" << endl
             << "\t            binning used in the analysis."                                           << endl
             << endl
		     << "Usage:   currentPlot [filelist] [yAxis_lenght] [zAxis_lenght] [cut[%]]"                << endl
		     << "Options: --help:       display this help message and exit"                             << endl
             << endl;
		return 0;
	} 

    cout << "Reading the input..." << endl;
    // Retrieving arguments
    stringstream ssy, ssz , ssc;
    double yUp, zUp, percCut;
    ssy << argv[2];
    ssy >> yUp;
    ssz << argv[3];
    ssz >> zUp;
    ssc << argv[4];
    ssc >> percCut;

    // Retrieve file names
    vector<string> listOfFiles;
    ifstream filelist( argv[1] );
    string name;
    while ( filelist >> name ) listOfFiles.push_back(name);

    // retrieve calibration parameters
    double m, q;
    stringstream ssm, ssq;
    ssm << listOfFiles.at(0);
    ssm >> m;
    ssq << listOfFiles.at(1);
    ssq >> q;

    // Retrieve channel number
    stringstream ssch;
    int ch;
    ssch << listOfFiles.at(2);
    ssch >> ch;

    // Retrieve tier2 file
    TFile infile( listOfFiles.at(listOfFiles.size()-2).c_str() , "READ" );
    if ( infile.IsZombie() ) { cout << "Bad input file! Quitting..." << endl; return 0; }

    // Retrieve current amplitude A
    TTree * currentTree = (TTree*)infile.Get("GEMDCurrentPSA_1")->Clone();
    vector<double> * A = new vector<double>;
    currentTree->SetBranchAddress( "A" , &A );

    // Retrieve energy E
    TTree * energyTree = (TTree*)infile.Get("GEMDEnergyGauss_1")->Clone();
    vector<double> * E = new vector<double>;
    energyTree->SetBranchAddress( "energy" , &E );

    // Retrieve list
    TFile infileList( listOfFiles.at(listOfFiles.size()-1).c_str() , "READ" );
    TEventList * list = (TEventList*)infileList.Get("listofselected")->Clone(); 
    
    // Retrieve master tree
//    TTree * masterTree = (TTree*)infile.Get("tree");
    
    // A/E vs E plot
    TH2D plot( "currentplot" , "currentplot" , 8000 , 1000 , 3500 , 1000 , 0 , yUp );
        plot.SetStats(kFALSE);
        plot.SetTitle(listOfFiles.at(listOfFiles.size()-2).c_str());
        plot.GetXaxis()->SetTitle("Energy [keV]");
        plot.GetYaxis()->SetTitle("A/E");
        plot.GetXaxis()->SetRangeUser(1520,1720);
        plot.GetZaxis()->SetRangeUser(0,zUp);
    
    // A/E histogram for DEP
    TH1D AEplot( "AEplot" , "Double escape peak @ 1592 keV" , 1000 , yUp/10 , yUp/4 );
        AEplot.SetStats(kFALSE);
        AEplot.GetXaxis()->SetTitle("A/E");
        AEplot.GetYaxis()->SetTitle("counts");
    
    // A/E histogram for MSE (Bi)
    TH1D AEplotBi( "AEplotBi" , "212Bi peak @ 1620 keV" , 1000 , yUp/10 , yUp/4 );
        AEplotBi.SetStats(kFALSE);
        AEplotBi.GetXaxis()->SetTitle("A/E");
        AEplotBi.GetYaxis()->SetTitle("counts");
    
    // Fill with data
    cout << "Filling the histograms... " << flush;
    int N = currentTree->GetEntries();
    double Ec; // calibrated energy
    double E1min = 1591.5;
    double E1max = 1593.5;
    double E2min = 1619.7;
    double E2max = 1621.7;

	double start = omp_get_wtime();

//#pragma omp parallel for shared(plot,AEplot,AEplotBi) firstprivate(currentTree,energyTree,list,E,A)
    for ( int i = 0 ; i < N ; i++ ) {

//#pragma omp critical
{
        currentTree->GetEntry(i);
        energyTree->GetEntry(i);
}       
        if ( list->Contains(i) ) {
            
			Ec = E->at(ch)*m + q;
//#pragma omp critical
            plot.Fill( Ec , A->at(ch) / E->at(ch) );
            
			if ( Ec > E1min && Ec < E1max ) {         // Energy selection DEP
                //plot.Fill( Ec , A->at(ch) / E->at(ch) );
//#pragma omp critical
                AEplot.Fill( A->at(ch) / E->at(ch) );
            }
            
            if ( Ec > E2min && Ec < E2max ) {         // Energy selection MSE
                //plot.Fill( Ec , A->at(ch) / E->at(ch) );
//#pragma omp critical
                AEplotBi.Fill( A->at(ch) / E->at(ch) );
            }
        }
    }

	double time = omp_get_wtime() - start;
	cout << "Took " << time << " seconds." << endl;

    // Compute the integral function of AEplot and AEplotBi
    cout << "Computing the integral function..." << endl;
    double totEv = AEplot.Integral();
    double totEvBi = AEplotBi.Integral();
    double cut = yUp/10;
    double intg = 0;
    double intgBi = 0;
    double savedCut = 0;
    double savedIntgBi = 0;

    // Graphs for the integral functions
    TGraph perc;
        perc.SetMarkerStyle(7);
        perc.SetLineColor(kBlue);
    TGraph percBi;
        percBi.SetMarkerStyle(7);
        percBi.SetLineColor(kRed);

    int npoints = 1000;
    for ( int i = 0 ; i < npoints ; i++ ) {
        perc.SetPoint( i , cut , 100-intg );    // cut goes from max to min
        percBi.SetPoint( i , cut , intgBi );    // cut goes from min to max
        cut   += (yUp)*3/(20*npoints);
        intg   = AEplot.Integral(0,AEplot.FindBin(cut))*100/totEv;
        intgBi = AEplotBi.Integral(0,AEplotBi.FindBin(cut))*100/totEvBi;
        if ( 100-intg > percCut-1 && 100-intg < percCut+1 ) {savedCut = cut; savedIntgBi = intgBi;}
    }
        perc.SetTitle("Integral function");
        perc.GetXaxis()->SetTitle("Cut on A/E");
        perc.GetYaxis()->SetTitle("Integral [%]");
        perc.GetXaxis()->SetRangeUser(AEplot.GetXaxis()->GetXmin(),AEplot.GetXaxis()->GetXmax());

/// DRAW SECTION ///////////////////////////////////////////////////////////////
    
    cout << "Drawing..." << endl;
    // Framework
    TApplication app( "app" , &argc , argv );
    TCanvas can( "currentCan" , "Current plot" , 1200 , 800 );
    can.Divide(2,2);
    can.cd(1);
    gPad->SetGrid();
    
    // AE vs E plot
    plot.Draw("COLZ");
//  Faster way to plot the A/E vs E graph 
//    masterTree->SetEventList(list);
//    masterTree->Draw( "GEMDCurrentPSA_1.A[1]/GEMDEnergyGauss_1.energy[1]:GEMDEnergyGauss_1.energy[1]*0.321159-0.76>>hist(1000,1000,3500,1000,0,1.5)" , "" , "COLZ" );
   
    // lines on AE vs E plot
    TLine lineAE_1( E1min , plot.GetYaxis()->GetXmin() , E1min , plot.GetYaxis()->GetXmax() ); 
    TLine lineAE_2( E1max , plot.GetYaxis()->GetXmin() , E1max , plot.GetYaxis()->GetXmax() );
    TLine lineAE_3( E2min , plot.GetYaxis()->GetXmin() , E2min , plot.GetYaxis()->GetXmax() );
    TLine lineAE_4( E2max , plot.GetYaxis()->GetXmin() , E2max , plot.GetYaxis()->GetXmax() );

    lineAE_1.Draw();
    lineAE_2.Draw();
    lineAE_3.Draw();
    lineAE_4.Draw();

    // integral functions
    can.cd(3);
    gPad->SetGrid();
    perc.Draw("AC");
    percBi.Draw("SAME");
    
    // graphic stuff
    TLine line( AEplot.GetXaxis()->GetXmin() , percCut , savedCut , percCut );
    TLine line2( savedCut , savedIntgBi , savedCut , percCut );
    TLine line3( savedCut , savedIntgBi , AEplot.GetXaxis()->GetXmax() , savedIntgBi );
        line.SetLineWidth(2);
        line2.SetLineWidth(2);
        line3.SetLineWidth(2);
        line.SetLineStyle(7);
        line2.SetLineStyle(7);
        line3.SetLineStyle(7);
        line.Draw();
        line2.Draw();
        line3.Draw();
    
    double roundedIntgBi = savedIntgBi;
    roundedIntgBi *= 10;
    roundedIntgBi = round(roundedIntgBi);
    roundedIntgBi /= 10;
    stringstream ssint;
    string textperc;
    ssint << roundedIntgBi;
    ssint >> textperc;
    textperc = " " + textperc + " %";

    TText text( AEplot.GetXaxis()->GetXmax() , savedIntgBi , textperc.c_str() );
    text.SetTextAlign(12);
    text.Draw();

    TLegend leg( 0.1533 , 0.4400 , 0.3292 , 0.5612 );
        leg.AddEntry( &perc , "DoubleEscape" , "L" );
        leg.AddEntry( &percBi , "212Bi" , "L" );
        leg.SetBorderSize(0);
        leg.SetFillColorAlpha(0,0.7);
        leg.SetTextSize(0.035);
        leg.Draw();
    
    // rebin factor for a better looking peak histograms
    int rebin = 2;

    // DEP
    can.cd(2);
    gPad->SetGrid();
    //gPad->SetLogy();

	// compute FWHM
	int bin1 = AEplot.FindFirstBinAbove(AEplot.GetMaximum()*1./2);
	int bin2 = AEplot.FindLastBinAbove(AEplot.GetMaximum()*1./2);
	float FWHM = AEplot.GetBinCenter(bin2) - AEplot.GetBinCenter(bin1);
	
	stringstream sssig;
	string textsig;
	sssig << FWHM << scientific;
	sssig >> textsig;
	textsig = "FWHM = " + textsig;
	TText textSig( 0.30 , 400 , textsig.c_str() );
	textSig.SetTextAlign(12);

    AEplot.Rebin(rebin);
    AEplot.Draw();
    
    // graphic to show the discarded events
    TH1D histFill( "fillhist" , "fillhist" , AEplot.GetNbinsX() , AEplot.GetXaxis()->GetXmin() , AEplot.GetXaxis()->GetXmax() );
    for ( int i = 0 ; i < AEplot.FindBin(savedCut) ; i++ ) {
        histFill.SetBinContent( i , AEplot.GetBinContent(i) );
    }

    histFill.SetFillStyle(3001);
    histFill.SetFillColor(kBlue);
    histFill.Draw("SAME");
	textSig.Draw("SAME");
    
    // MSE
    can.cd(4);
    gPad->SetGrid();
    //gPad->SetLogy();
    AEplotBi.Rebin(rebin);
    AEplotBi.Draw();
    
    // Graphic to show the discarded events
    TH1D histFillBi( "fillhistbi" , "fillhistbi" , AEplot.GetNbinsX() , AEplot.GetXaxis()->GetXmin() , AEplot.GetXaxis()->GetXmax() );
    for ( int i = 0 ; i < AEplot.FindBin(savedCut) ; i++ ) {
        histFillBi.SetBinContent( i , AEplotBi.GetBinContent(i) );
    }

    histFillBi.SetFillStyle(3001);
    histFillBi.SetFillColor(kBlue);
    histFillBi.Draw("SAME");
    
    // Save .pdf
    listOfFiles.at(listOfFiles.size()-2) += ".pdf";
    can.SaveAs( listOfFiles.at(listOfFiles.size()-2).c_str() );
       
	//TCanvas * can2d = new TCanvas("can2d","can2d",1);
	//can2d->cd();
	//plot.Draw("COLZ");

    // RUN!
    app.Run(kTRUE);
    
    return 0;
}
