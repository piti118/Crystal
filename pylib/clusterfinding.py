from itertools import izip
from pprint import pprint
from math import log,exp,sqrt
from util import average
import pymongo

hex_ring_step = (1,0)
hex_move_list = [(0,1),(-1,0),(-1,-1),(0,-1),(1,0),(1,1)]
hex_side_size = lambda x: x+1 #this measure how many step we should walk

sq_ring_step = (1,1)
sq_move_list = [(0,-1),(-1,0),(0,1),(1,0)]
sq_side_size = lambda x: 2*x + 1
#return list of l and k for ring size of ringsize and center at center
def ring(center, ringsize, move_list,side_size,ring_step):
    #ringlist = [(0,0)]
    ringlist = [center]
    for iring in xrange(1,ringsize+1):
        #build it ring by ring
        cp = tuple(iring*x+y for x,y in izip(ring_step,center))
        for step in move_list:
            #walks along move_list
            for i in xrange(1,side_size(iring)): 
                #walks only n-1 step corner is taken care off
                np = tuple(x+y for x,y in izip(step,cp))
                ringlist.append(np)
                cp = np
    #pprint(ringlist)
    return ringlist

def hex_ring(center,ringsize):
    return ring(center,ringsize,hex_move_list,hex_side_size,hex_ring_step)

def square_ring(center,ringsize):
    return ring(center,ringsize,sq_move_list,sq_side_size,sq_ring_step)

def find_bump(clist):
    #return crystal with the highest deposited energy
    return max(clist,key=lambda x:x['dedx'])
    pass

def find_cluster(clist,ring,ringsize=1):
    #return list of crystal to be used in cluster finding
    bump = find_bump(clist) #find crystal with highest dedx
    bump_center = tuple([bump['l'],bump['k']])
    ringset = set(ring(bump_center,ringsize))
    #note: ignoring out of bound ring
    toReturn = filter(lambda x: tuple([x['l'],x['k']]) in ringset,clist)
    return toReturn

def sqr_pos(clist):
    xlist = (x['x'] for x in clist)
    elist = [sqrt(x['dedx']) for x in clist]
    x = average( xlist, elist )
    ylist = (y['y'] for y in clist)
    y = average( ylist, elist )
    return (x,y)
    
def lin_pos(clist):
    xlist = (x['x'] for x in clist)
    elist = [x['dedx'] for x in clist]
    x = average( xlist, elist )
    ylist = (y['y'] for y in clist)
    y = average( ylist, elist )
    return (x,y)
def log_pos(clist,a=4.0):
    #return tuple of x,y
    bump = find_bump(clist)
    E_bump = bump['dedx']
    E_cutoff = E_bump*exp(-1*a)
    wlist = []
    for c in clist:
        E = c['dedx']
        #this is to avoid log of 0 evaluation
        #also there are some event with energy less than 1.0
        w = 0 if E < E_cutoff or E_bump < 10.0 else a+log(E/E_bump)
        wlist.append(w)
    x = average((x['x'] for x in clist), wlist)
    y = average((y['y'] for y in clist), wlist)
    return (x,y)

def distance(a,b):
    return sqrt((a[0]-b[0])**2+(a[1]-b[1])**2) 
def writecluster(col,alg,evt,pos,a=0):
    beam_center = (evt['beamx'],evt['beamy'])
    dis = distance( beam_center, pos)
    col.insert({
        'alg':alg,
        'a':a,
        'x':pos[0],
        'y':pos[1],
        'dis':dis,
        'bx':evt['beamx'],
        'by':evt['beamy'],
        'angle': evt['angle'],
        'evt':evt['_id']})  
      
def go():
    db = pymongo.Connection().hex
    i=0
    col = db.cluster
    col.remove()
    for evt in db.raw.find():
        clist = evt['dedx']
        cluster = find_cluster(clist,hex_ring,ringsize=2)
        #linear
        lin_center = lin_pos(cluster)
        writecluster(col,'lin',evt,lin_center)
        #sqr
        sqr_center = sqr_pos(cluster)
        writecluster(col,'sqr',evt,sqr_center)
        #log
        alist = [2.0,3.0,4.0,5.0,6.0]
        for a in alist:
            log_center = log_pos(cluster,a=a)
            writecluster(col,'log',evt,log_center,a)
        i+=1
        if(i%1000==0): print i
    print 'creating index'
    col.create_index([('alg',pymongo.ASCENDING),('a',pymongo.ASCENDING)])
def main():
    go()
if __name__ == '__main__':
    main()
  