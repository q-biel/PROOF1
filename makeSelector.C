{
TFile *f = TFile::Open("Data.root");
TTree *d = (TTree *) f->Get("Data");
d->MakeSelector("MySelector");
}
