void plotY() {

    TGraph * FWHM = new TGraph( "plotY.txt" , "%lg %lg %*lg %*lg" );
    TGraph * FWTM = new TGraph( "plotY.txt" , "%lg %*lg %lg %*lg" );
    TGraph * FWFM = new TGraph( "plotY.txt" , "%lg %*lg %*lg %lg" );

    TGraph * ratioTH = new TGraph();
    TGraph * ratioFH = new TGraph();
    ratioTH->SetName("ratioTH");
    ratioFH->SetName("ratioFH");

    double * x = FWHM->GetX();
    double * yH = FWHM->GetY();
    double * yT = FWTM->GetY();
    double * yF = FWFM->GetY();

    for ( int i = 0 ; i < FWHM->GetN() ; i++ ) {
        ratioTH->SetPoint( i , x[i] , yT[i]/yH[i] );
        ratioFH->SetPoint( i , x[i] , yF[i]/yH[i] );
    }
	
	ratioTH->SetTitle("Gaussianity");
    ratioTH->GetXaxis()->SetTitle("Iterations");
	ratioTH->SetMarkerStyle(20);
	ratioTH->SetMarkerColor(kViolet+2);
	ratioTH->SetMarkerSize(1.5);
	ratioTH->SetLineColor(kViolet+2);
    
	ratioFH->SetMarkerStyle(29);
	ratioFH->SetMarkerColor(kYellow+2);
	ratioFH->SetMarkerSize(1.5);
	ratioFH->SetLineColor(kYellow+2);
    ratioTH->GetYaxis()->SetRangeUser(1,6);

    FWHM->SetTitle("Resolution");
    FWHM->GetYaxis()->SetRangeUser(0,17);
    FWHM->GetYaxis()->SetTitle("[keV]");
    FWHM->GetXaxis()->SetTitle("Iterations");

    FWHM->SetMarkerStyle(33);
    FWHM->SetMarkerColor(kGreen+3);
    FWHM->SetMarkerSize(1.5);
	FWHM->SetLineColor(kGreen+3);

    FWTM->SetMarkerStyle(22);
    FWTM->SetMarkerColor(kBlue);
    FWTM->SetMarkerSize(1.5);
	FWTM->SetLineColor(kBlue);

    FWFM->SetMarkerStyle(23);
    FWFM->SetMarkerColor(kRed);
    FWFM->SetMarkerSize(1.5);
	FWFM->SetLineColor(kRed);

    TCanvas * can = new TCanvas("can","can",1);
    can->Divide(2,1);
    can->cd(1);
    gPad->SetGrid();

    FWHM->Draw("AP");
    FWTM->Draw("PSAME");
    FWFM->Draw("PSAME");
	
	TLegend * leg = new TLegend(0.24,0.77,0.42,0.88);
	leg->SetTextAlign(22);
	leg->SetTextSize(0.037);
	leg->AddEntry(FWHM,"FWHM","p");
	leg->AddEntry(FWTM,"FWTM","p");
	leg->AddEntry(FWFM,"FWFM","p");
	leg->Draw();

    can->cd(2);
	gPad->SetGrid();
    ratioTH->Draw("AP");
    ratioFH->Draw("PSAME");
  
	TLine * lineTH = new TLine( ratioTH->GetXaxis()->GetXmin() , 1.82 , ratioTH->GetXaxis()->GetXmax() , 1.82 );
	TLine * lineFH = new TLine( ratioTH->GetXaxis()->GetXmin() , 2.38 , ratioTH->GetXaxis()->GetXmax() , 2.38 );

	lineTH->SetLineWidth(2);
	lineTH->SetLineStyle(9);
	lineTH->SetLineColor(kViolet+2);

	lineFH->SetLineWidth(2);
	lineFH->SetLineStyle(9);
	lineFH->SetLineColor(kYellow+2);

	lineTH->Draw();
    lineFH->Draw();
	
	TLegend * leg2 = new TLegend(0.14,0.77,0.45,0.88);
	leg2->SetTextAlign(22);
	leg2->SetTextSize(0.037);
	leg2->AddEntry(ratioTH,"FWTM/FWHM","p");
	leg2->AddEntry(ratioFH,"FWFM/FWHM","p");
	leg2->Draw();

	return;
}
