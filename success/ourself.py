# -*- coding: utf-8 -*-
"""
Created on Sun Mar 17 10:58:51 2019

@author: BZK
"""
'''
使用道路通过时间替代了路径长度的dijkstra算法
'''
import logging
import sys
import numpy as np
from collections import defaultdict
from heapq import heappop
from heapq import heappush


logging.basicConfig(level=logging.DEBUG,
                    filename='../logs/CodeCraft-2019.log',
                    format='[%(asctime)s] %(levelname)s [%(funcName)s: %(filename)s, %(lineno)d] %(message)s',
                    datefmt='%Y-%m-%d %H:%M:%S',
                    filemode='a')


def main():
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

    MAX = float('inf')
    np.random.seed(123)
    # dijkstra算法实现，有向图和路由的源点作为函数的输入，最短路径最为输出
    #def dijkstra(graph,num, src,end):
    


    def dijkstra(graph_, from_, to_):
        g = defaultdict(list)
        for l,r,c in graph_:
            g[l].append((c,r))
        q, seen, mins = [(0,from_,())], set(), {from_: 0}
        while q:
            (cost,v1,path) = heappop(q)
            if v1 not in seen:
                seen.add(v1)
                path = (v1, path)
                if v1 == to_:
                    return (cost, path)

                for c, v2 in g.get(v1, ()):
                    if v2 in seen: continue
                    prev = mins.get(v2, None)
                    next = cost + c
                    if prev is None or next < prev:
                        mins[v2] = next
                        heappush(q, (next, v2, path))
        return MAX
    
    def dataRead(car_path, road_path, cross_path): 
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
    
    carData, crossData, roadData = dataRead(car_path, road_path, cross_path)
    
    graphData = []
    
    for i in range(len(roadData)):
        if roadData[i][-1] == 1:
            graphData.append([roadData[i][4], roadData[i][5], roadData[i][1]])
            graphData.append([roadData[i][5], roadData[i][4], roadData[i][1]])
        elif roadData[i][-1] == 0:
            graphData.append([roadData[i][4], roadData[i][5], roadData[i][1]])
    
    
    carRoute = []
    
    for carNum in range(len(carData)):
        result = dijkstra(graphData, carData[carNum][1], carData[carNum][2])
        sumarize = []
        while result[1] != ():
            sumarize.append(int(result[0]))
            if result[1] != ():
                result = result[1]
        sumarize.append(int(result[0]))
    
        numbers = len(sumarize)
        carRouteTmp = [carData[carNum][0]]
        car_speed = carData[carNum][3]
        low_add = 0 if car_speed == 8 else (800 if car_speed == 6 else (1600 if car_speed == 4 else 2400))
        high_add = 800 if car_speed == 8 else (1600 if car_speed == 6 else (2400 if car_speed == 4 else 3200)) 
        carRouteTmp.append(carData[carNum][-1]+int(np.random.uniform(low_add,high_add)))
        for i in range(1, numbers - 1):
            for j in range(len(roadData)):
                if ((roadData[j][-3] == sumarize[numbers - i] and \
                     roadData[j][-2] == sumarize[numbers - i -1]) or \
                    (roadData[j][-2] == sumarize[numbers - i] and roadData[j][-3] == sumarize[numbers - i -1])):
                    carRouteTmp.append(roadData[j][0])
        carRoute.append(tuple(carRouteTmp))

    f = open(answer_path,'w+')
    f.truncate()
    for line in carRoute:
        f.write(str(line) + '\n')
    f.close()

if __name__ == "__main__":
    main()