from matplotlib import pyplot as plt
import math
from math import radians
import pymongo
from pprint import pprint
def initdb():
    dbs={}
    dbs['square'] = pymongo.Connection().square
    dbs['hexbig'] = pymongo.Connection().hexbig
    dbs['hexsmall'] = pymongo.Connection().hexsmall
    return dbs

def closeto(x,tl=0.000001): return {'$lt':x+tl,'$gt':x-tl}
def key_alg_a(alg,a):return {'alg':alg,'a':closeto(a)}
def sort_angle(): return [('angle',1)]
def f_Emean(x): return (x['E']['mean'],radians(x['angle']))
def f_Eres(x): return (1-x['E']['res'],radians(x['angle']))
def f_rmean(x): return (x['dis']['mean'],radians(x['angle']))
def f_rsd(x): return (x['dis']['sd'],radians(x['angle']))

def mongoplot(ax,f,cur,label):
    #f(x) should return tuple of r and theta(in radians)
    r,theta = zip(*[f(x) for x in cur])
    ax.plot(theta,r,label=label)
def plot_by_det(dbs):
    for key,db in dbs.items():
        col = db.cluster_sum
        plt.rc('font',size=10)
        fig = plt.figure(figsize=(10,10))
        fig.suptitle(key,fontsize=15)
        ax={}
        ax['E_mean'] = fig.add_subplot(2,2,1,polar=True)
        ax['E_res'] = fig.add_subplot(2,2,2,polar=True)
        ax['r_mean'] = fig.add_subplot(2,2,3,polar=True)
        ax['r_sd'] = fig.add_subplot(2,2,4,polar=True)
        alglist = col.distinct('alg')
        for alg in alglist:
            alist = col.find({'alg':alg}).distinct('a')
            for a in alist:
                cur = [ x for x in col.find(key_alg_a(alg,a)).sort(sort_angle()) ]
                label = alg+'_'+str(a)
                mongoplot(ax['E_mean'],f_Emean,cur,label=label)
                mongoplot(ax['E_res'],f_Eres,cur,label=label)
                mongoplot(ax['r_mean'],f_rmean,cur,label=label)
                mongoplot(ax['r_sd'],f_rsd,cur,label=label)
            ###
        ###
        ax['E_mean'].set_rmax(105);ax['E_mean'].set_title('mean E(MeV)')
        ax['E_res'].set_rmax(1.0);ax['E_res'].set_title('1 - rms/mean')
        ax['r_mean'].set_rmax(4.5);ax['r_mean'].set_title('Mean Shift(cm)')
        ax['r_sd'].set_rmax(2.5);ax['r_sd'].set_title('Shift RMS(cm)')
        ax['r_sd'].legend(loc='lower right',prop={'size':8})
        fig.savefig(key+'.pdf')
def comparedet(dbs):
    plt.rc('font',size=10)
    fig = plt.figure(figsize=(10,10))
    fig.suptitle('Compare by crystal shape',fontsize=15)
    ax={}
    ax['E_mean'] = fig.add_subplot(2,2,1,polar=True)
    ax['E_res'] = fig.add_subplot(2,2,2,polar=True)
    ax['r_mean'] = fig.add_subplot(2,2,3,polar=True)
    ax['r_sd'] = fig.add_subplot(2,2,4,polar=True)
    for key,db in dbs.items():
        col = db.cluster_sum
        keyy = key_alg_a('log',2)
        cur = [x for x in col.find(keyy).sort(sort_angle())]
        mongoplot(ax['E_mean'],f_Emean,cur,label=key)
        mongoplot(ax['E_res'],f_Eres,cur,label=key)
        mongoplot(ax['r_mean'],f_rmean,cur,label=key)
        mongoplot(ax['r_sd'],f_rsd,cur,label=key)
    ax['E_mean'].set_rmax(105);ax['E_mean'].set_title('mean E(MeV)')
    ax['E_res'].set_rmax(1.0);ax['E_res'].set_title('1 - rms/mean')
    ax['r_mean'].set_rmax(4.5);ax['r_mean'].set_title('Mean Shift(cm)')
    ax['r_sd'].set_rmax(2.5);ax['r_sd'].set_title('Shift RMS(cm)')
    ax['r_sd'].legend(loc='lower right',prop={'size':8})
    
    fig.savefig('bydet.pdf')
  
def main():
    dbs = initdb()
    comparedet(dbs)

    

if __name__ == '__main__':
    main()