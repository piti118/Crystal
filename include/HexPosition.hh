#ifndef HEXPOSITION_H
#define HEXPOSITION_H value
#include <vector>
//#include <cassert>
class HexPosition{
public:
  int l;
  int k;
  int ringno;
  int segmentno;
  HexPosition():l(0),k(0),ringno(0),segmentno(0){}
  HexPosition(int l, int k, int rno, int sno):
    l(l),k(k),ringno(rno),segmentno(sno){}
};
class LK{
public:
  LK(int l, int k):l(l),k(k){}
  int l; int k;
  void operator += (LK x) {l+=x.l;k+=x.k;}
};
class HexPositionGenerator{
public:
  std::vector<LK> step;//step vector for going around hex

  HexPositionGenerator():step(){initstep();}
  virtual ~HexPositionGenerator(){}
  
  std::vector<HexPosition> generate(int nring);

private:
  
  void initstep(){
    step.clear();
    addstep(-1,0);//down left
    addstep(-1,-1);//left
    addstep(0,-1);//up left
    addstep(1,0);//up right
    addstep(1,1);//right
    addstep(0,1);//down right
  }
  inline void addstep(int l, int k){
    LK x(l,k);step.push_back(x);
  }
};

#endif