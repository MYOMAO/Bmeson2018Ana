#include <fstream>

void addtochain(TChain* root, TString dir, TString filelist, Int_t imerge, Int_t nmerge)
{
  ifstream inf(filelist.Data());
  for(int i=0;i<(imerge+nmerge);i++)
    {
      TString filename;
      inf>>filename;
      if(i<imerge) continue;
      cout<<filename<<endl;
      root->Add(Form("%s/%s",dir.Data(),filename.Data()));
    }
}
