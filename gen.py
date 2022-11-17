numPlanets = 20
minSize, maxSize = (5,20)
maxX, maxY = (2000,2000)

from sys import argv
import random 

with open(argv[1],'w') as file:
    file.write(str(numPlanets) + '\n')
    random.seed()
    diff = maxSize-minSize
    for i in range(numPlanets):
        s = minSize + (diff*random.random())
        x = random.random()*maxX
        y = random.random()*maxY
        file.write('{} {} {}\n'.format(s,x,y))