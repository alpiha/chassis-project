import aStar
import os


class Point:
    def __init__(self, name, x, y):
        self.name = name
        self.x = x
        self.y = y


"""
TODO: incomingRoute shall be linked to A* algorthim 
world: is where the point are being placed 
movement: is how long the car is on the route 
"""
incomingRoute = aStar.RUN()
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
    lastDirection = ""
    for test in movement:
        print("current point: " + currentPoint.name)
        # print("last: " + lastDirection)
        # print("next point: " + nextPoint.name)
        if currentPoint.x == nextPoint.x and currentPoint.y > nextPoint.y:
            direction = "north"
        elif currentPoint.x == nextPoint.x and currentPoint.y < nextPoint.y:
            direction = "south"
        elif currentPoint.y == nextPoint.y and currentPoint.x > nextPoint.x:
            direction = "west"
        elif currentPoint.y == nextPoint.y and currentPoint.x < nextPoint.x:
            direction = "east"
        # print("new: " + direction)
        ROUTE(lastDirection, direction)
        i += 1
        currentPoint = nextPoint
        if currentPoint == movement[-1]:
            break
        nextPoint = movement[i + 1]
        lastDirection = direction


"""
Comparing the last direction with the Next Direction. The logic is like, example:
if the car came from north (moving down) and the next direction is west (moving left), then it should be turning Left. 
"""


def ROUTE(last, next):
    if last == next or last == "":
        print("forward")
        ROUTE_PRINT("F")
    if (
        last == "north"
        and next == "west"
        or last == "south"
        and next == "east"
        or last == "west"
        and next == "south"
        or last == "east"
        and next == "north"
    ):
        print("Left")
        ROUTE_PRINT("L")
    if (
        last == "north"
        and next == "east"
        or last == "south"
        and next == "west"
        or last == "west"
        and next == "north"
        or last == "east"
        and next == "south"
    ):
        print("right")
        ROUTE_PRINT("R")


"""
Wring the string to the file each time it is called. "a" = append. 
"""


def ROUTE_PRINT(string):
    f = open("route.txt", "a")
    f.write(string)
    f.close()


"""
Setting up the movement the car shall be taking. If aChar from the incoming route matches with the name of the point that is in the worldmap, 
it is added to the movement of the car.
"""


def SET_UP_CAR_MOVEMENT():
    for aChar in incomingRoute:
        # print(aChar)
        for point in worldMap:
            if aChar == point.name:
                movement.append(point)
            #    print(point.name)


"""
Setting up a worldmap. 
"""


def SET_UP_WORLDMAP():
    a = Point("A", 0, 0)
    b = Point("B", 1, 0)
    c = Point("C", 2, 0)
    d = Point("D", 2, 1)
    e = Point("E", 3, 1)
    f = Point("F", 1, 2)
    g = Point("G", 2, 2)
    h = Point("H", 2, 3)
    i = Point("I", 0, 4)
    j = Point("J", 1, 4)
    k = Point("K", 2, 0)
    l = Point("L", 3, 4)
    m = Point("M", 1, 5)
    n = Point("N", 3, 5)
    o = Point("O", 0, 6)
    p = Point("P", 1, 6)
    q = Point("Q", 0, 7)
    r = Point("R", 1, 7)
    s = Point("S", 3, 7)
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

"""


def RUN():
    os.remove("route.txt")
    SET_UP_WORLDMAP()
    SET_UP_CAR_MOVEMENT()
    GET_DIRECTION()


"""
Starting the application
"""
if __name__ == "__main__":
    RUN()
