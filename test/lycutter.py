import logging
import sys
from collections import defaultdict
from heapq import *
import re

def dataProcess(carPath, crossPath, roadPath):
    carData = []
    crossData = []
    roadData = []
    with open(carPath, 'r') as lines:
        for line in lines:
            line = line.split(',')
            if re.findall("\d+", line[0]) != []:
                line[0] = re.findall("\d+", line[0])[0]
            if re.findall("\d+", line[-1]) != []:
                line[-1] = re.findall("\d+", line[-1])[0]
            # for i in range(len(line)):
            #     line[i] = int(line[i].strip())
            carData.append(line)
    with open(roadPath, 'r') as lines:
        for line in lines:
            line = line.split(',')
            if re.findall("\d+", line[0]) != []:
                line[0] = re.findall("\d+", line[0])[0]
            if re.findall("\d+", line[-1]) != []:
                line[-1] = re.findall("\d+", line[-1])[0]
            roadData.append(line)
    with open(crossPath, 'r') as lines:
        for line in lines:
            line = line.split(',')
            if re.findall("\d+", line[0]) != []:
                line[0] = re.findall("\d+", line[0])[0]
            if re.findall("\d+", line[-1]) != []:
                line[-1] = re.findall("\d+", line[-1])[0]
            crossData.append(line)

    carData = carData[1: ]
    for i in range(len(carData)):
        for j in range(len(carData[i])):
            carData[i][j] = int(carData[i][j].strip())
    roadData = roadData[1: ]
    for i in range(len(roadData)):
        for j in range(len(roadData[i])):
            roadData[i][j] = int(roadData[i][j].strip())
    crossData = crossData[1: ]
    for i in range(len(crossData)):
        for j in range(len(crossData[i])):
            crossData[i][j] = int(crossData[i][j].strip())
            if crossData[i][j] == 1:
                crossData[i][j] = -1
    return carData, crossData, roadData

def dijkstra(edges, f, t):
    if f == 19 and t == 52:
        print("debug")
    g = defaultdict(list)
    for l,r,c in edges:
        g[l].append((c,r))

    q, seen, mins = [(0,f,())], set(), {f: 0}
    while q:
        (cost,v1,path) = heappop(q)
        if v1 not in seen:
            seen.add(v1)
            path = (v1, path)
            if v1 == t:

                return (cost, path)

            for c, v2 in g.get(v1, ()):
                if v2 in seen: continue
                prev = mins.get(v2, None)
                next = cost + c
                if prev is None or next < prev:
                    mins[v2] = next
                    heappush(q, (next, v2, path))



    return float("inf")


def Seek(carData, crossData, roadData):

    edges = []

    # 生成地图（双向图）
    for i in range(len(roadData)):
        if (roadData[i][-1] == 1):
            edges.append((str(roadData[i][-3]), str(roadData[i][-2]), roadData[i][1]))
            edges.append((str(roadData[i][-2]), str(roadData[i][-3]), roadData[i][1]))
        else:
            edges.append((str(roadData[i][-3]), str(roadData[i][-2]), roadData[i][1]))

    # 生成地图（单向图）
    # for i in range(len(roadData)):
    #     if (roadData[i][-1] == 1):
    #         edges.append((str(roadData[i][-3]), str(roadData[i][-2]), roadData[i][1]))

    # print("ok")

    carRoute = []
    # print(dijkstra(edges, "22", "2"))
    for carNum in range(len(carData)):

        # result = dijkstra(edges, "2", "31")
        result = dijkstra(edges, str(carData[carNum][1]), str(carData[carNum][2]))
        sumarize = []
        while result[1] != ():
            sumarize.append(int(result[0]))
            if result[1] != ():
                result = result[1]
        sumarize.append(int(result[0]))
        # print(sumarize)


        lengthSumarize = len(sumarize)
        carRouteTmp = [carData[carNum][0]]
        carRouteTmp.append(carData[carNum][-1])
        for i in range(1, lengthSumarize - 1):
            if carData[carNum][0] == 10054 and i == 5:
                print("debug")
            for j in range(len(roadData)):
                if ((roadData[j][-3] == sumarize[lengthSumarize - i] and roadData[j][-2] == sumarize[lengthSumarize - i -1]) or (roadData[j][-2] == sumarize[lengthSumarize - i] and roadData[j][-3] == sumarize[lengthSumarize - i -1])):
                    carRouteTmp.append(roadData[j][0])
        carRoute.append(tuple(carRouteTmp))
    # print(carRoute)

    return carRoute


car_path = './car.txt'
road_path = './road.txt'
cross_path = './cross.txt'
answer_path = './answer.txt'


carData, crossData, roadData = dataProcess(car_path, cross_path, road_path)
carRoute = Seek(carData, crossData, roadData)

with open(answer_path, 'w') as f:
    f.write('#(carId,StartTime,RoadId...)')
    f.write('\n')
    for i in range(len(carRoute)):
        for j in range(len(carRoute[i])):
            if j == 0:
                f.write('(')
            f.write(str(carRoute[i][j]))
            if j != len(carRoute[i]) - 1:
                f.write(', ')
            else:
                f.write(')')
        if i != len(carRoute) - 1:
            f.write('\n')
    f.close()

print("OK")
