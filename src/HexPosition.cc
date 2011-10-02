#include "HexPosition.hh"
std::vector<HexPosition> HexPositionGenerator::generate(int nring){
  std::vector<HexPosition> toReturn;
  for(int iring=0;iring<nring;iring++){//for eachring
    LK cp(iring,iring);
    //walking along step
    int segno = 0;
    for(unsigned int istep =0;istep<step.size();istep++){
      //walks iring steps for each side
      
      for(unsigned int iseg=0;iseg<iring;iseg++){
        cp+=step[istep];
        HexPosition thisPos(cp.l,cp.k,iring,segno);
        toReturn.push_back(thisPos);
        ++segno;
      }//end seg
    }//end step
  }//end ring
  //assert(toReturn.size()==1+nring*(nring-1)/2*6);
  return toReturn;
} 
