from itertools import izip
from pprint import pprint
from math import log,exp,sqrt
from util import average
import argparse
import pymongo
from clusterutil import *

def writecluster(col,alg,evt,pos,E,a=0):
    beam_center = (evt['beamx'],evt['beamy'])
    dis = distance( beam_center, pos)
    col.insert({
        'alg':alg,
        'a':a,
        'x':pos[0],
        'y':pos[1],
        'E':E,
        'dis':dis,
        'bx':evt['beamx'],
        'by':evt['beamy'],
        'angle': evt['angle'],
        'evt':evt['_id']})  
      
def go(dbname,type_):
    db = pymongo.Connection()[dbname]
    i=0
    col = db.cluster
    col.remove()
    ring = {'hex':hex_ring,'square':square_ring}[type_]

    for evt in db.raw.find():
        clist = evt['dedx']
        cluster = find_cluster(clist,ring,ringsize=2)
        #linear
        lin_center = lin_pos(cluster)
        E = totalE(cluster)
        writecluster(col, 'lin', evt, lin_center, E)
        #sqr
        sqr_center = sqr_pos(cluster)
        
        writecluster(col, 'sqr', evt,sqr_center, E)
        #log
        alist = [2.0,3.0,4.0,5.0,6.0]
        for a in alist:
            log_center = log_pos(cluster,a=a)
            writecluster(col,'log',evt,log_center,E,a)
        i+=1
        if(i%1000==0): print i
    print 'creating index'
    col.create_index([('alg',pymongo.ASCENDING),('a',pymongo.ASCENDING)])

def main():
    parser = argparse.ArgumentParser(description='do cluster finding')
    parser.add_argument('--db', action="store", help='database name',required=True)
    parser.add_argument('--type', action="store",default=None, help='type[hex|square]')
    arg = parser.parse_args()

    arg.type = 'hex' if arg.type is None and 'hex' in arg.db else arg.type
    
    arg.type = 'square' if arg.type is None and 'square' in arg.db else arg.type
    if arg.type not in ['hex','square']:
        print 'Specify type [hex|square]'    
    print arg
    go(arg.db,arg.type)

if __name__ == '__main__':
    main()
  