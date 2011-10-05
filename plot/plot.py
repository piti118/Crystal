from matplotlib import pyplot as plt
import math
from math import radians
import pymongo
from pprint import pprint
from matplotlib.transforms import Affine2D

import mpl_toolkits.axisartist.floating_axes as floating_axes
import  mpl_toolkits.axisartist.angle_helper as angle_helper
from matplotlib.projections import PolarAxes
from mpl_toolkits.axisartist.grid_finder import FixedLocator, MaxNLocator, \
     DictFormatter
def initdb():
    dbs={}
    dbs['square'] = pymongo.Connection().square
    dbs['hexbig'] = pymongo.Connection().hexbig
    dbs['hexsmall'] = pymongo.Connection().hexsmall
    dbs['hexarea'] = pymongo.Connection().hexarea
    return dbs

def closeto(x,tl=0.000001): return {'$lt':x+tl,'$gt':x-tl}
def key_alg_a(alg,a):return {'alg':alg,'a':closeto(a)}
def sort_angle(): return [('angle',1)]
def f_Emean(x): return (x['E']['mean'],x['angle'])
def f_Eres(x): return (1-x['E']['res'],x['angle'])
def f_rmean(x): return (x['dis']['mean'],x['angle'])
def f_rsd(x): return (x['dis']['sd'],x['angle'])

def mongoplot(ax,f,cur,label):
    #f(x) should return tuple of r and theta(in radians)
    r,theta = zip(*[f(x) for x in cur])
    print label,r[:5]
    ax.plot(theta,r,label=label)

def add_first_quardrant_polar(fig,rect,rmax,thetamax=90,title=""):
 
    # scale degree to radians this make the title looks really nice
    tr_scale = Affine2D().scale(math.pi/180., 1.)

    #tr = tr_rotate + tr_scale + PolarAxes.PolarTransform()
    tr = tr_scale + PolarAxes.PolarTransform()
    #tr = PolarAxes.PolarTransform()
    grid_locator1 = angle_helper.LocatorD(9)
    tick_formatter1 = angle_helper.FormatterDMS()

    grid_locator2 = MaxNLocator(10)

    ra0, ra1 = 0, thetamax
    cz0, cz1 = 0, rmax
    grid_helper = floating_axes.GridHelperCurveLinear(tr,
                                        extremes=(ra0, ra1, cz0, cz1),
                                        grid_locator1=grid_locator1,
                                        grid_locator2=grid_locator2,
                                        tick_formatter1=tick_formatter1,
                                        tick_formatter2=None,
                                        )

    ax1 = floating_axes.FloatingSubplot(fig, rect, grid_helper=grid_helper)
    ax1.grid(True)
    fig.add_subplot(ax1)

    # adjust axis
    ax1.axis["left"].set_axis_direction("bottom")
    ax1.axis["right"].set_axis_direction("top")

    ax1.axis["bottom"].set_visible(False)
    ax1.axis["top"].set_axis_direction("bottom")
    ax1.axis["top"].toggle(ticklabels=True, label=True)
    ax1.axis["top"].major_ticklabels.set_axis_direction("top")
    ax1.axis["top"].label.set_axis_direction("top")

    # create a parasite axes whose transData in RA, cz
    aux_ax = ax1.get_aux_axes(tr)

    aux_ax.patch = ax1.patch # for aux_ax to have a clip path as in ax
    ax1.patch.zorder=0.9 # but this has a side effect that the patch is
                        # drawn twice, and possibly over some other
                        # artists. So, we decrease the zorder a bit to
                        # prevent this.
    ax1.set_title(title)
    return aux_ax
    #return ax1, aux_ax

def setup_axis(fig):
    ax={}
    ax['E_mean'] = add_first_quardrant_polar(fig,221,105,90,'mean E(MeV)')
    ax['E_res'] = add_first_quardrant_polar(fig,222,1.0,90,'1 - rms/mean')
    ax['r_mean'] = add_first_quardrant_polar(fig,223,4.5,90,'Mean Shift(cm)')
    ax['r_sd'] = add_first_quardrant_polar(fig,224,2.5,90,'Shift RMS(cm)')
    return ax

def decorate_ax(ax):
    #do nothing
    pass
    
def make_legend(ax):
    ax['r_sd'].legend(loc='lower right',prop={'size':8})
    
def plot_by_det(dbs):
    for key,db in dbs.items():
        col = db.cluster_sum
        plt.rc('font',size=10)
        fig = plt.figure(figsize=(10,10))
        fig.suptitle(key,fontsize=15)
        ax=setup_axis(fig)
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
        decorate_ax(ax)
        make_legend(ax)
        fig.savefig(key+'.pdf')

def comparedet(dbs):
    plt.rc('font',size=10)
    fig = plt.figure(figsize=(10,10))
    fig.suptitle('Compare by crystal shape',fontsize=15)
    ax=setup_axis(fig)
    for key,db in dbs.items():
        col = db.cluster_sum
        keyy = key_alg_a('log',2)
        cur = [x for x in col.find(keyy).sort(sort_angle())]
        mongoplot(ax['E_mean'],f_Emean,cur,label=key)
        mongoplot(ax['E_res'],f_Eres,cur,label=key)
        mongoplot(ax['r_mean'],f_rmean,cur,label=key)
        mongoplot(ax['r_sd'],f_rsd,cur,label=key)
    decorate_ax(ax)
    make_legend(ax)
    
    fig.savefig('bydet.pdf')
  
def main():
    dbs = initdb()
    plot_by_det(dbs)
    comparedet(dbs)

    

if __name__ == '__main__':
    main()