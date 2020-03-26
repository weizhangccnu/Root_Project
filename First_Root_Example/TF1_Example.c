{
    TCanvas *c = new TCanvas("c", "Title");     //new a Canvas 

    f1 = new TF1("f1", "sin(x)/x", 0, 15);      //new a TF1 function

    f1->SetLineColor(4);                        //set line color 
    f1->SetLineWidth(4);                        //set line width 
    f1->SetLineStyle(4);                        //set line style 

    f1->SetTitle("My first Root Plot");     
    f1->GetXaxis()->SetTitle("Time [ms]");
    f1->GetXaxis()->CenterTitle();

    f1->GetYaxis()->SetTitle("Value [mV]");
    f1->GetYaxis()->CenterTitle();

    f1->GetYaxis()->SetRangeUser(-1.4, 1.4);
    f1->Draw();
     
    f2 = new TF1("f2", "sin(x)", 0, 15);
    f2->SetLineColor(2); 
    f2->SetLineWidth(4); 
    f2->SetLineStyle(8); 
    f2->SetTitle("My second Root Plot");     
    f2->GetXaxis()->SetTitle("Time");
    f2->GetYaxis()->SetTitle("Value");
    f2->GetYaxis()->SetRangeUser(-1.4, 1.4);
    //f2->SetLegend(TLegend* 1){fLegend = 1};
    f2->Draw("SAME"); 
    
    auto legend = new TLegend(0.78, 0.8, 0.9, 0.9);
    legend->AddEntry(f1, "sin(x)/x");
    legend->AddEntry(f2, "sin(x)");
    legend->Draw("SAME");

    c->Update();

    //gSystem->ProcessEvents();

    TImage *img = TImage::Create();

    img->FromPad(c);
    img->WriteImage("Sin_Waveform.png");

    c->SaveAs("Sin_Waveform.pdf");
}
