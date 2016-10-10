// ROOT macro to find peaks (GERDA) 
// Discard all bad events using a TEventList

void peakAnalysis( string filename2 , string filenamelist ) {
   
    TFile * file0 = new TFile(filename2.c_str());
    TFile * file1 = new TFile(filenamelist.c_str());

    TTree * tree = (TTree*)file0->Get("tree");
    TEventList * listsel = (TEventList*)file1->Get("listofselected");

    TCanvas * can = new TCanvas( "canPeaks" , "Peak Analysis" , 7500 , 5500 , 900 , 600 );
    //can->SetLogy();
    can->SetGrid();
    can->cd();

    tree->SetEventList(listsel);
    tree->Draw("GEMDEnergyGauss_1.energy[1]>>htemp(10000,10000,10000)");
    TH1D * spectrum = (TH1D*)gDirectory->Get("htemp");

    spectrum->SetTitle("Energy Spectrum");
    spectrum->SetXTitle("Energy [keV]");
    spectrum->SetYTitle("Counts");
    //spectrum->SetStats(kFALSE);
    
    TSpectrum analyzer( 8 , 2 );
        // arg1: max number of peaks
        // arg2: resolution between peaks
    analyzer.Search( spectrum , 2 , "" , 0.003);
        // arg2: sigma of the searched peaks
        // arg3: options
        // arg4: peaks with amplitude less than threshold*highest_peak are discarded

    vector<double> peaks;
    for ( int i = 0 ; i < analyzer.GetNPeaks() ; i++ ) {
        peaks.push_back(analyzer.GetPositionX()[i]);
    }
    sort(peaks.begin(),peaks.end());

    cout << endl << "########## PEAKS ##########" << endl;
    for ( int i = 0 ; i < peaks.size() ; i++ ) cout << i+1 << "\t" << peaks.at(i) << endl;

    cout << "###########################" << endl << endl;
   
    ofstream file("calib.txt");

    file << "// calibration"                                     << endl
         << "// ch\tkeV"                                         << endl
         << peaks.at(0) << "\t1460.882 // 40K"                   << endl
         << peaks.at(1) << "\t1512.700 // 212Bi"                 << endl
         << peaks.at(2) << "\t1592.515 // 208Tl (double escape)" << endl
         << peaks.at(3) << "\t1620.738 // 212Bi"                 << endl
         << peaks.at(4) << "\t2103.513 // 208Tl (single escape)" << endl
         << peaks.at(5) << "\t2614.511 // 208Tl";

    return;
}
