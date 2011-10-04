import util
import pymongo
def closeto(x,tl=0.0000001):
    return {'$lt':x+tl,'$gt':x-tl}

def main():
    db = pymongo.Connection().hex
    col = pymongo.Connection().hex.cluster
    col.create_index('angle')
    col.create_index('alg')
    col.create_index([('alg',pymongo.ASCENDING),('a',pymongo.ASCENDING),('angle',pymongo.ASCENDING)])
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
                s = util.stat(rlist);
                m = s.tomap()
                m['angle']=angle
                m['a']=a
                m['alg']=alg
                db.cluster_sum.insert(m)
                i+=1
                if i%10==0: print (alg,angle,a)

if __name__ == '__main__':
    main()