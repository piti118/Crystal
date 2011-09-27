import cairo
import math
def drawhex(ctx,center,width,color=(0,0,1.0)):
  cx,cy = center
  deg30 = math.pi/6
  deg60 = math.pi/3
  r = width/math.cos(deg30)
  corners = []
  for iside in range(6):
    theta = iside*deg60+deg30
    x = cx+r*math.cos(theta)
    y = cy+r*math.sin(theta)
    corners.append((x,y))
  ctx.move_to(*corners[-1])
  for corner in corners:
    ctx.line_to(*corner)

  ctx.set_source_rgb(0,0,0)
  ctx.set_line_width(2)
  ctx.stroke_preserve()
  ctx.set_source_rgb(*color)
  ctx.close_path()
  ctx.fill()

def hexlist(numring):#todo: make this a generator
  walkingvector = [
    (-1,0), #down left
    (-1,-1), #left
    (0,-1), #up left
    (1,0), #up right
    (1,1), #right
    (0,1), #down right
  ]
  print numring
  #generate set of vector
  ringvec = []
  center = (0,0)
  for iring in xrange(numring):
    if iring==0:
      ringvec.append([center])
      continue
    veclist = []
    prevcenter = (iring,iring)
    for iside in xrange(6):
      currentstep = walkingvector[iside]
      for iseg in xrange(iring):
        thiscenter = tuple(map(lambda x,y: x+y,prevcenter,currentstep))
        veclist.append(thiscenter)
        prevcenter = thiscenter
    ringvec.append(veclist)
  return ringvec

def lk2xy(lk):
  l,k = lk
  x = (l+k)*math.cos(math.pi/3)
  y = (l-k)*math.sin(math.pi/3)
  return (x,y)

def main():
  ssize = 10000
  surface = cairo.PDFSurface('hex.pdf',ssize,ssize)
  ctx = cairo.Context(surface)
  numring = 100
  
  center = (ssize/2,ssize/2)
  hlist = hexlist(numring)
  hexsize = 50;gap = 3
  r = 2*hexsize+gap
  
  color = [tuple([1-x*0.02])*3 for x in xrange(10)]
  iring = 0
  for clist in hlist:
    for c in clist:
      newc = tuple((r*x)+cx for x,cx in zip(lk2xy(c),center))
      #print newc
      drawhex(ctx,newc,hexsize,color=color[iring%len(color)])
    iring=iring+1
  
  #draw circles
  for i in xrange(100):
    ctx.set_source_rgb(1.0,0,0)
    ctx.arc(center[0],center[1],1*r*i,0,2*math.pi)
    ctx.stroke()
  surface.finish()
  
if __name__ == '__main__':
  main()