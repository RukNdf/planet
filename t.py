"""
Matplotlib Animation Example

author: Jake Vanderplas
email: vanderplas@astro.washington.edu
website: http://jakevdp.github.com
license: BSD
Please feel free to use and modify this, but keep the above information. Thanks!
"""
from matplotlib import pyplot as plt
from matplotlib import animation


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
            
xs = [dot.cord[0] for dot in dots]
ys = [dot.cord[1] for dot in dots]
for dot in dots:
    xs += [cord[0] for cord in dot.next]
    ys += [cord[1] for cord in dot.next]

fig = plt.figure()
ax = plt.axes(xlim=(min(xs), max(xs)), ylim=(min(ys), max(ys)))
dx = [dot.cord[0] for dot in dots]
dy =[dot.cord[1] for dot in dots]
d = ax.scatter(dx, dy, s=[dot.size for dot in dots])
def animate(i):
    for dot in dots:
        dot.move()
    d.set_offsets([[dot.cord[0],dot.cord[1]] for dot in dots])
    return d

anim = animation.FuncAnimation(fig, animate, frames=200, interval=50)

plt.show()
