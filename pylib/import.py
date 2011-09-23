import yaml
from yaml import CLoader
import pymongo
f = open("output.yaml");
d = yaml.load_all(f,Loader=CLoader)
coll = pymongo.Connection().crystal.crystal
print "Clearing collection..."
coll.remove()
for s in d:
    if s is not None:
      print str(s["runno"])+"/"+str(s["eventno"]);
      coll.insert(s)
