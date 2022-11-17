"""
Matplotlib Animation Example

author: Jake Vanderplas
email: vanderplas@astro.washington.edu
website: http://jakevdp.github.com
license: BSD
Please feel free to use and modify this, but keep the above information. Thanks!
"""
import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation
import math

# Initializing number of dots
N = 25


# Creating dot class
class dot(object):
    def __init__(self, size, x, y):
        self.size = size
        self.cord = (x,y)
        self.next = []

    def move(self):
        if self.next == []:
            return
        self.cord = self.next.pop(0)


from sys import argv
dots = []
with open(argv[1]) as file:
    count = 0
    line = file.readline()
    while 'STEP' not in line:
        if line == '\n':
            line = file.readline()
            continue
        line = line.split(' ')
        count += 1
        dots += [dot(float(line[2]),float(line[4]),float(line[6]))]
        line = file.readline()

    step = 0
    while line != '':
        step += 1
        count = 0
        while 'STEP' not in line:
            if line == '':
                line = file.readline()
                break
            if line in ('\n'):
                line = file.readline()
                continue

            line = line.split()
            dots[count].next += [(float(line[4]),float(line[6]))]
            count += 1
            line = file.readline()
        line = file.readline()
            
xs = [dot.cord[0] for dot in dots]#+[0,30]
ys = [dot.cord[1] for dot in dots]#+[0,30]
for dot in dots:
    xs += [cord[0] for cord in dot.next]
    ys += [cord[1] for cord in dot.next]
#print(xs)

fig = plt.figure()
ax = plt.axes(xlim=(min(xs), max(xs)), ylim=(min(ys), max(ys)))
dx = [dot.cord[0] for dot in dots]
dy =[dot.cord[1] for dot in dots]
d = ax.scatter(dx, dy, s=[dot.size for dot in dots])
def animate(i):
    for dot in dots:
        dot.move()
    #print(dots[dot].cord[0])
    #d = ax.scatter([dot.cord[0] for dot in dots], [dot.cord[1] for dot in dots])#, s=[dot.size for dot in dots])
    d.set_offsets([[dot.cord[0],dot.cord[1]] for dot in dots])
    return d

# call the animator.  blit=True means only re-draw the parts that have changed.
anim = animation.FuncAnimation(fig, animate, frames=200, interval=50)

plt.show()
exit()
'''
def animate(i):
    for dot in dots:
        dot.move()
    print(dots[0].cord)
    d = ax.scatter(dx, dy, s=[dot.size for dot in dots])
    return d

anim = animation.FuncAnimation(fig, animate, frames=200, interval=20)

plt.show()

exit()
'''
'''
def update(num, my_ax):
    # the following corresponds to whatever logic must append in your code
    # to get the new coordinates of your points
    # in this case, we're going to move each point by a quantity (dx,dy,dz)
    dx, dy, dz = np.random.normal(size=(3,N_points), loc=0, scale=1) 
    debug_text.set_text("{:d}".format(num))  # for debugging
    x,y,z = graph._offsets3d
    new_x, new_y, new_z = (x+dx, y+dy, z+dz)
    graph._offsets3d = (new_x, new_y, new_z)
    for t, new_x_i, new_y_i, new_z_i in zip(annots, new_x, new_y, new_z):
        # animating Text in 3D proved to be tricky. Tip of the hat to @ImportanceOfBeingErnest
        # for this answer https://stackoverflow.com/a/51579878/1356000
        x_, y_, _ = proj3d.proj_transform(new_x_i, new_y_i, new_z_i, my_ax.get_proj())
        t.set_position((x_,y_))
    return [graph,debug_text]+annots


# create N_points initial points
x,y,z = np.random.normal(size=(3,N_points), loc=0, scale=10)

fig = plt.figure(figsize=(5, 5))
ax = fig.add_subplot(111, projection="3d")
graph = ax.scatter(x, y, z, color='orange')
debug_text = fig.text(0, 1, "TEXT", va='top')  # for debugging
annots = [ax.text2D(0,0,"POINT") for _ in range(N_points)] 

# Creating the Animation object
ani = animation.FuncAnimation(fig, update, fargs=[ax], frames=100, interval=50, blit=True)
plt.show()
'''