import pymongo
import clusterutil as cu
db = pymongo.Connection().hexbig
col = db.raw
#get the first event
evt = col.find_one({'eventno':80,'runno':0})
shape = cu.HexShape
clist = cu.cluster_expansion(evt['dedx'],shape)
print clist
