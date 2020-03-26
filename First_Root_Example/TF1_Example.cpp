{
    TCanvas *c = new TCanvas("c", "Title");     //new a Canvas 

    f1 = new TF1("f1", "sin(x)/x", 0, 15);      //new a TF1 function

    f1->SetLineColor(4);                        //set line color 
    f1->SetLineWidth(4);                        //set line width 
    f1->SetLineStyle(4);                        //set line style 

    f1->SetTitle("My first Root Plot");         //add title for f1 function
    f1->GetXaxis()->SetTitle("Time [ms]");      //add x-axis label
    f1->GetXaxis()->CenterTitle();              //set xlable on center

    f1->GetYaxis()->SetTitle("Value [mV]");     //add y-axis label
    f1->GetYaxis()->CenterTitle();              //set ylabel on center

    f1->GetYaxis()->SetRangeUser(-1.4, 1.4);    //set y-axis range
    f1->Draw();                                 //draw f1 function
     
    f2 = new TF1("f2", "sin(x)", 0, 15);        //new a f2 function
    f2->SetLineColor(2);                        //set line color
    f2->SetLineWidth(4);                        //set line width
    f2->SetLineStyle(8);                        //set line style
    f2->SetTitle("My second Root Plot");        //set f2 function title
    f2->GetXaxis()->SetTitle("Time");           //set x-axis label
    f2->GetYaxis()->SetTitle("Value");          //set y-axis label
    f2->GetYaxis()->SetRangeUser(-1.4, 1.4);    //set y-axis range
    f2->Draw("SAME");                           //draw f1 and f2 in the same canvas
    
    auto legend = new TLegend(0.78, 0.8, 0.9, 0.9);         //add legend
    legend->AddEntry(f1, "sin(x)/x");                       //add f1 legend
    legend->AddEntry(f2, "sin(x)");                         //add f2 legend
    legend->Draw();                                         //draw legend

    c->Update();                                //update canvas

    TImage *img = TImage::Create();             //define a TImage pointer

    img->FromPad(c);                
    img->WriteImage("Sin_Waveform.png");        //save canvas to png image

    c->SaveAs("Sin_Waveform.pdf");              //save canvas to pdf 
}
