import pymongo
from pprint import pprint
from math import sqrt, floor
from itertools import izip,repeat,imap
from UserDict import UserDict
numrow = numcol = 9
centerrow = (numrow-1)/2
centercol = (numcol-1)/2
drow = 3;#mm
dcol = 3;#mm

class Object(UserDict):
    pass

class StatAcc:
  nn=0
  s=0
  s2=0
  
  def n(self): return self.nn;
  def sum(self): return self.s
  def sumsq(self): return self.s2
  def mean(self): return self.s*1.0/self.nn if self.nn!=0 else 0
  def sqmean(self): return self.s2*1.0/self.nn if self.nn!=0 else 0
  def var(self): return self.sqmean()**2-self.mean()**2
  def sd(self): return self.sqrt(self.var())
  def res(self): return self.sd()/self.mean() if self.mean()!=0 else -1.0
  
  @staticmethod
  def acc(stat,x):
    stat.nn+=1
    stat.s+=x
    stat.s2+=x**2
  
class Stat:
  def __init__(self,n,s,s2):
    self.n = n
    self.sum = s
    self.sumsq = s2
    self.mean = s*1.0/n
    self.sqmean = s2*1.0/n
    self.var = self.sqmean-self.mean**2
    self.sd = sqrt(self.var)
    self.res = self.sd/self.mean if self.mean!=0 else -1
  
  @staticmethod
  def statmap(n,s,s2):
    return Stat(n,s,s2).__dict__;

def logPosition(calor,r=1):
  numrow = 5
  return (0.,0.)

def centerFilter(calor,r=1):
  maxirow = centerrow+r
  minirow = centerrow-r
  maxicol = centercol+r
  minicol = centercol-r
  return filter(lambda x: minirow <= x['row'] <= maxirow and minicol <= x['col'] <= maxicol,calor)

def crystalR(row,col):
  nrow = centerrow-row;
  ncol = centercol-col;
  return sqrt((drow*nrow)**2+(dcol*ncol)**2)

def crystalX(col):
  return (centercol-col)*dcol;
  
def crystalY(row):
  return (centerrow-row)*drow;  

def eadd(x,y):#element wise add for iterable
  if x is None: x = (0,)*len(y)
  return tuple(a+b for (a,b) in izip(x,y))

def teadd(x,y):#tuple of tutple elementwise add
  if x is None: x = (None,)*len(y)
  return tuple(eadd(a,b) for (a,b) in izip(x,y))

def deadd(x,y):#dictionary elementwise add
  if x is None: x = dict( (key,None) for key in y.keys())
  return dict( (key,eadd(x[key],y[key]) ) for key in y.keys() )

def dstat(it):#stat for dictionary
  #assume key are the same
  result = reduce(lambda x,y: deadd(x, dict( (key,(1,val,val**2)) for key,val in y.items())) , it, None )
  return dict((key,Stat.statmap(*val)) for key,val in result.items())
  
def tstat(it):#stat for generator of tuple of values
  result = reduce(lambda x,y: teadd(x, tuple((1,z,z**2) for z in y) ), it, None )
  toReturn = tuple(Stat(n,s,s2) for (n,s,s2) in result) #n+1 because eadd start at 0 not 1
  return toReturn
  
def stat(it):
  (n,s,s2) = reduce(lambda x,y: eadd(x,(1,y,y**2)), it, None)
  o = Stat(n,s,s2)
  return o
    
def linearPosition(calor,r=1):
  fcalors =centerFilter(calor,r)
  #print [(x['row'],x['col']) for x in fcalors]
  totalE = sum(x['dedx'] for x in fcalors)
  wEx = sum(x['dedx']*crystalX(x['col']) for x in fcalors) 
  wEy = sum(x['dedx']*crystalY(x['row']) for x in fcalors)
  toReturn = Object()
  toReturn.x = x = wEx/totalE
  toReturn.y = y = wEy/totalE
  toReturn.r = sqrt(x**2+y**2)
  toReturn.totalE = totalE
  #print (toReturn.x,toReturn.y)
  return toReturn
  
