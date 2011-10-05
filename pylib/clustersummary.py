import util
import pymongo
import argparse
from mongoutil import closeto
from clusterutil import *

def go(dbname,type_):
    db = pymongo.Connection()[dbname]
    col = db.cluster
    col.create_index('angle')
    col.create_index('alg')
    col.create_index([('alg',pymongo.ASCENDING),('a',pymongo.ASCENDING),('angle',pymongo.ASCENDING)])
    db.cluster_sum.drop()
    angles = [x for x in col.distinct('angle')]
    algs = [x for x in col.distinct('alg')]
    tl = 0.001#tolerance
    i = 0
    for alg in algs:
        alist = col.find({'alg':alg}).distinct('a')
        for a in alist:
            for angle in angles:
                key = {'alg':alg,'a':closeto(a),'angle':closeto(angle)}
                rlist = [x['dis'] for x in col.find(key)]
                sr = util.stat(rlist)
                elist = [x['E'] for x in col.find(key)]
                se = util.stat(elist)
                m = {}
                m['dis'] = sr.tomap()
                m['E'] = se.tomap()
                m['angle']=angle
                m['a']=a
                m['alg']=alg
                db.cluster_sum.insert(m)
                i+=1
                if i%10==0: print (alg,angle,a)
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