from util import Object,dstat
from pprint import pprint
a = [dict((str(i),i+j) for i in xrange(10) ) for j in xrange(10) ]
pprint([i for i in a])
pprint(dstat(a))