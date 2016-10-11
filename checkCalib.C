// ROOT macro to check energy calibration (GERDA) 
// Discard all bad events using a TEventList

void perform( TFile *file0 , TFile *file1 , double m , double q , bool subtr );

void checkCalib( string filename , bool subtr = false ) {

    //cout << filename << endl << flush;
    ifstream filelist( filename.c_str() );
    vector<string> listOfFiles;
    string name;
    while ( filelist >> name ) listOfFiles.push_back(name);
    
    //for ( int i = 0 ; i < listOfFiles.size() ; i++ ) cout << listOfFiles.at(i) << endl;

    float m, q;
    stringstream ssm, ssq;
    ssm << listOfFiles.at(0);
    ssm >> m;
    ssq << listOfFiles.at(1);
    ssq >> q;

    TFile * file0 = new TFile( listOfFiles.at(listOfFiles.size()-2).c_str() );
    TFile * file1 = new TFile( listOfFiles.at(listOfFiles.size()-1).c_str() );

    perform( file0 , file1 , m , q , subtr );

    return;
}

void checkCalib( string filename , string filelist , double m , double q , bool subtr = false ) {

    TFile * file0 = new TFile( filename.c_str() );
    TFile * file1 = new TFile( filelist.c_str() );

    perform( file0 , file1 , m , q , subtr );

    return;
}

// Analyzing function, subtr = false : plot original histogram and background
//                     subtr = true  : plot the new spectrum and the selected region
void perform( TFile *file0 , TFile *file1 , double m , double q , bool subtr ) {

    // retrieve tree and list
    TTree * tree = (TTree*)file0->Get("tree");
    TEventList * listsel = (TEventList*)file1->Get("listofselected");
    
    // calibrate
    tree->SetEventList(listsel);
    tree->Draw( Form("GEMDEnergyGauss_1.energy[1]*%f+%f>>htemp(20000,1000,3000)" , m , q ) );
    TH1D * spectrum = (TH1D*)gDirectory->Get("htemp");

    spectrum->SetTitle("Energy Spectrum");
    spectrum->SetXTitle("Energy [keV]");
    spectrum->SetYTitle("Counts");
    //spectrum->SetStats(kFALSE);

    if (subtr==false) {
        spectrum->GetYaxis()->SetRangeUser(0,300);
        spectrum->Draw();
    }

    // peak analyzer
    TSpectrum analyzer( 8 , 2 );
    analyzer.Search( spectrum , 2 , "nodraw" , 0.004 );
    
    // dump peak positions
    list<double> peaks(10);
    for ( int i = 0 ; i < analyzer.GetNPeaks() ; i++ ) peaks.push_back(analyzer.GetPositionX()[i]);
    peaks.sort();

    cout << endl << "########## PEAKS ##########" << endl;
    for ( int i = analyzer.GetNPeaks()-1 ; i >= 0 ; i-- ) {
        cout << analyzer.GetNPeaks()-i << " -> " << analyzer.GetPositionX()[i] << endl;
    }
    cout << "###########################" << endl << endl;
    
    // copy TH1 into array to compute background
    int nbins = spectrum->GetNbinsX();
    double *source = new double[nbins];
    for ( int i = 0 ; i < nbins ; i++ ) source[i] = spectrum->GetBinContent(i+1); 
    
    // compute background
    analyzer.Background(source,nbins,100,TSpectrum::kBackDecreasingWindow,TSpectrum::kBackOrder2,kTRUE,TSpectrum::kBackSmoothing7,kFALSE);
    
    // retrieve background and draw it
    TH1D * bkg = new TH1D("bkg","bkg",nbins,1000,3000);
    bkg->SetStats(kFALSE);
    for ( int i = 0 ; i < nbins ; i++ ) bkg->SetBinContent( i+1 , source[i] );
    bkg->SetLineColor(kRed);
    if (subtr==false) bkg->Draw("SAME");
    
    // compute new, cleaned spectrum
    TH1D * subHist = new TH1D("sub","sub",nbins,1000,3000);
    subHist->SetStats(kFALSE);
    subHist->SetTitle("Subtracted histogram");
    for ( int i = 0 ; i < nbins ; i++ ) subHist->SetBinContent( i+1 , spectrum->GetBinContent(i+1) - source[i] );
    if (subtr==true) subHist->Draw();
    
    // get the highest peak
    int binmin = subHist->FindBin(2580);
    int binmax = subHist->FindBin(2640);
    TH1D * peakTl = new TH1D( "peakTl" , "peakTl" , binmax-binmin , 2580 , 2640 );
    for ( int i = 1 ; i <= binmax-binmin ; i++ ) peakTl->SetBinContent(i,subHist->GetBinContent(binmin+i-1));
    peakTl->SetBinContent(0,0);
    peakTl->SetBinContent(binmax-binmin+1,0);
        peakTl->SetStats(kFALSE);
        peakTl->SetFillStyle(3001);
        peakTl->SetFillColor(kBlue);
        peakTl->SetLineColor(kBlack);
    if (subtr==true) peakTl->Draw("SAME");

    // compute FWHM
    int bin1 = peakTl->FindFirstBinAbove(peakTl->GetMaximum()*1./2);
    int bin2 = peakTl->FindLastBinAbove(peakTl->GetMaximum()*1./2);
    //cout << bin1 << "   " << bin2 << endl;
    //cout << peakTl->GetXaxis()->GetBinCenter(bin2) << "   " << peakTl->GetXaxis()->GetBinCenter(bin1) << endl;
    cout << "FWHM: " << peakTl->GetBinCenter(bin2) - peakTl->GetBinCenter(bin1) << endl;

    // compute FWTM
    bin1 = peakTl->FindFirstBinAbove(peakTl->GetMaximum()*1./10);
    bin2 = peakTl->FindLastBinAbove(peakTl->GetMaximum()*1./10);
    cout << "FWTM: " << peakTl->GetBinCenter(bin2) - peakTl->GetBinCenter(bin1) << endl;

    // compute FWFM
    bin1 = peakTl->FindFirstBinAbove(peakTl->GetMaximum()*1./50);
    bin2 = peakTl->FindLastBinAbove(peakTl->GetMaximum()*1./50);
    cout << "FWFM: " << peakTl->GetBinCenter(bin2) - peakTl->GetBinCenter(bin1) << endl << endl;
 
    return;
}
