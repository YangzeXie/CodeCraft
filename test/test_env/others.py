import logging
import sys
from collections import defaultdict
from heapq import *
import re
import numpy as np

logging.basicConfig(level=logging.DEBUG,
                    filename='../logs/CodeCraft-2019.log',
                    format='[%(asctime)s] %(levelname)s [%(funcName)s: %(filename)s, %(lineno)d] %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S',
                    filemode='a')

def main():
    
    np.random.seed(123)
    if len(sys.argv) != 5:
        logging.info('please input args: car_path, road_path, cross_path, answerPath')
        exit(1)

    car_path = sys.argv[1]
    road_path = sys.argv[2]
    cross_path = sys.argv[3]
    answer_path = sys.argv[4]

    logging.info("car_path is %s" % (car_path))
    logging.info("road_path is %s" % (road_path))
    logging.info("cross_path is %s" % (cross_path))
    logging.info("answer_path is %s" % (answer_path))

    def dataProcess(carPath, crossPath, roadPath):
        # load .txt files
        carData = []
        crossData = []
        roadData = []

        lines = open(car_path, 'r').read().split('\n')[1:]
        for line in lines:
            line = line.replace(' ', '').replace('\t', '')[1:-1].split(',')
            line = [int(i) for i in line]
            carData.append(line)

        lines = open(road_path, 'r').read().split('\n')[1:]
        for line in lines:
            line = line.replace(' ', '').replace('\t', '')[1:-1].split(',')
            line = [int(i) for i in line]
            roadData.append(line)
        lines = open(cross_path, 'r').read().split('\n')[1:]
        for line in lines:
            line = line.replace(' ', '').replace('\t', '')[1:-1].split(',')
            line = [int(i) for i in line]
            crossData.append(line)

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

        carRoute = []
        for carNum in range(len(carData)):

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
            car_speed = carData[carNum][3]
            low_add = 0 if car_speed == 8 else (100 if car_speed == 6 else (200 if car_speed == 4 else 300))
            high_add = 100 if car_speed == 8 else (200 if car_speed == 6 else (300 if car_speed == 4 else 400)) 
            carRouteTmp.append(carData[carNum][-1]+int(np.random.uniform(low_add,high_add)))
            for i in range(1, lengthSumarize - 1):
                for j in range(len(roadData)):
                    if ((roadData[j][-3] == sumarize[lengthSumarize - i] and roadData[j][-2] == sumarize[lengthSumarize - i -1]) or (roadData[j][-2] == sumarize[lengthSumarize - i] and roadData[j][-3] == sumarize[lengthSumarize - i -1])):
                        carRouteTmp.append(roadData[j][0])
            carRoute.append(tuple(carRouteTmp))
        # print(carRoute)

        return carRoute

    carData, crossData, roadData = dataProcess(car_path, cross_path, road_path)
    carRoute = Seek(carData, crossData, roadData)

    with open(answer_path, 'w') as f:
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

if __name__ == "__main__":
    main()
