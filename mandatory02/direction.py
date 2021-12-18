import aStar

class Point: 
    def __init__(self, name, x,y):
        self.name = name
        self.x = x
        self.y = y

"""
incomingRoute: is getting the result form the A* algorthim 
routeFile: is where the route is saved
worldMap: is where the point are being placed 
movement: is how long the car is on the route 
"""
#incomingRoute = "AIJMNS"
incomingRoute = aStar.run_Astar()
routeFile = "mandatory02/route.txt"
worldMap = []
movement = []

"""
Getting the Next direction (North, South, West and East) the car should be going. furthermore saving the last direction. 
There after calling ROUTE()) method
"""
def GET_DIRECTION():
    i = 0
    currentPoint = movement[0]
    nextPoint = movement[1]
    nextDirection =""
    lastDirection = ""
    for element in movement:  
        print(" ")
        print("current point: " + currentPoint.name)
        print("next point: " + nextPoint.name)
        if (currentPoint.x == nextPoint.x and currentPoint.y > nextPoint.y):
            nextDirection = "north"
        elif (currentPoint.x == nextPoint.x and currentPoint.y < nextPoint.y):    
            nextDirection = "south"
        elif(currentPoint.y == nextPoint.y and currentPoint.x > nextPoint.x):
            nextDirection = "west"
        elif (currentPoint.y == nextPoint.y and currentPoint.x < nextPoint.x):   
            nextDirection = "east"    
        ROUTE(lastDirection, nextDirection)
        i += 1
        currentPoint = nextPoint
        print("next direction: " + nextDirection)
        print("last direction: " + lastDirection)
        if currentPoint == movement[-1]:
            break
        nextPoint = movement[i+1]
        lastDirection = nextDirection


"""
Comparing the last direction with the Next Direction. The logic is like, example:
if the car came from north (moving down) and the next direction is west (moving left), then it should be turning Left. 
"""
def ROUTE(last, next):
    if(last == next or last == ""):
        WRITE_FILE("'F',","a")
    if(last == "north" and next == "west" or last == "south" and next == "east" or last == "west" and next == "south" or last == "east" and next == "north"):
        WRITE_FILE("'L',","a")
    if(last == "north" and next == "east" or last == "south" and next == "west" or last == "west" and next == "north" or last == "east" and next == "south"):
        WRITE_FILE("'R',","a")

"""
Setting up the movement the car shall be taking. If aChar from the incoming route matches with the name of the point that is in the worldmap, 
it is added to the movement of the car.
"""
def SET_UP_CAR_MOVEMENT():
    for aChar in incomingRoute:
        for point in worldMap:
           if(aChar == point.name):
               movement.append(point)
               print("point:", point.name , " is added to world map")


"""
Setting up a worldmap. 
"""
def SET_UP_WORLDMAP():
    a = Point("A",0,0)
    b = Point("B",1,0)
    c = Point("C",2,0)
    d = Point("D",2,1)
    e = Point("E",3,1)
    f = Point("F",1,2)
    g = Point("G",2,2)
    h = Point("H",2,3)
    i = Point("I",0,4)
    j = Point("J",1,4)
    k = Point("K",2,4)
    l = Point("L",3,4)
    m = Point("M",1,5)
    n = Point("N",3,5)
    o = Point("O",0,6)
    p = Point("P",1,6)
    q = Point("Q",0,7)
    r = Point("R",1,7)
    s = Point("S",3,7)
    worldMap.append(a)
    worldMap.append(b)
    worldMap.append(c)
    worldMap.append(d)
    worldMap.append(e)
    worldMap.append(f)
    worldMap.append(g)
    worldMap.append(h)
    worldMap.append(i)
    worldMap.append(j)
    worldMap.append(k)
    worldMap.append(l)
    worldMap.append(m)
    worldMap.append(n)
    worldMap.append(o)
    worldMap.append(p)
    worldMap.append(q)
    worldMap.append(r)
    worldMap.append(s)


"""
Wring the string to the file each time it is called. Commands: "a" = append, w = overwrite. 
"""
def WRITE_FILE(string, command):
    file = open(routeFile, command)
    file.write(string)
    file.close()

"""
Print to see that the method is called. 
WRITE_FILE: to empty the file
SET_UP_WORLDMAP: points with coordinates
SET_UP_CAR_MOVEMENT: reading the route, and adding world map coordinates that matches 
GET_DIRECTION: starting to check what the cars direction is
"""
def run_Direction():
    print("Direction Algorithm is running")
    WRITE_FILE("","w")
    SET_UP_WORLDMAP()
    SET_UP_CAR_MOVEMENT()
    GET_DIRECTION()

"""
Starting the application
"""
if __name__ == "__main__":
   run_Direction()
