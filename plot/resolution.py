import sys
import matplotlib
batchmode = '-w' not in sys.argv
matplotlib.use('Agg') if batchmode else None
from matplotlib.pyplot import figure, show, rc, grid
import plotapp
import pymongo
import numpy as np

import math

def deg2rad(deg):
  return deg*math.pi/180.0
db = pymongo.Connection().crystal
summaries = db.summary.find()
r,theta = zip(*((
  100*(1-x['dedx']['res']),
  deg2rad(x['angle']
  )) for x in summaries))
title = "100*(1-sigma/mean) vs Angle"

ax = plotapp.radplot(**locals())

show()
