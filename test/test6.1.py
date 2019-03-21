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
import pandas as pd
import numpy as np
from collections import defaultdict
from collections import Counter


car_path = './car.txt'
road_path = './road.txt'
cross_path = './cross.txt'
answer_path = './answer.txt'

MAX = float('inf')
# dijkstra算法实现，有向图和路由的源点作为函数的输入，最短路径最为输出
#def dijkstra(graph,num, src,end):



def dijkstra():
    for v in range(n) :
        fina[v] = 0;
        d[v] = arcs[v0][v]
    d[v0] = 0;
    fina[v0] = 1;
    
    for i in range(1,n) :
        MIN = MAX
        for k in range(n) :
            if fina[k] == 0:
                if d[k] < MIN:
                    v = k;
                    MIN = d[k]
        fina[v] = 1;
        for k in range(n) :
            if (fina[k] == 0 and (MIN+arcs[v][k] < d[k])):
                p[k] = v
                d[k] = MIN + arcs[v][k]


def Print(sec, n):
    p2 = [0]*100
    t = 0
    j = v1
    
    while(p[j] != -1) :
        p2[t] = p[j]
        t = t+1
        j = p[j]
    
    cross = []
    path = []
    
    cross.append(v1+1)
    for k in range(0,t) :
        cross.append(p2[k]+1)
    cross.append(v0+1)
    cross.reverse()
    
    for i in range(len(cross)-1):
        a,b = cross[i],cross[i+1]
        if a < b:
            path.append(int(road_data[(road_data._from == a )& (road_data.to == b)].id))
        else:
            path.append(int(road_data[(road_data._from == b )& (road_data.to == a)].id))
    
    return path   

car_data = pd.read_csv(car_path,sep = ',',index_col=False,header=0)
car_data.columns = ['id','start','to','speed','planTime']
car_data = car_data.rename(columns=lambda x: x.replace("(","").replace(')','').replace('#',''))
car_data.id = car_data.id.apply(lambda x: x.replace("(","").replace(')',''))
car_data.planTime = car_data.planTime.apply(lambda x: x.replace(')',''))
car_data.id = car_data.id.apply(int)
car_data.start = car_data.start.apply(int)
car_data.to = car_data.to.apply(int)
car_data.speed = car_data.speed.apply(int)
car_data.planTime = car_data.planTime.apply(int)
car_data.set_index(['id'], inplace =True)


road_data = pd.read_csv(road_path,sep = ',',index_col=False,header=0)
road_data.columns = ['id','length','speed','channel','_from', 'to', 'isDuplex']
road_data = road_data.rename(columns=lambda x: x.replace("(","").replace(')','').replace('#',''))
road_data.id = road_data.id.apply(lambda x: x.replace("(","").replace(')',''))
road_data.isDuplex = road_data.isDuplex.apply(lambda x: x.replace(')',''))
road_data.id = road_data.id.apply(int)
road_data.length = road_data.length.apply(int) 
road_data.speed = road_data.speed.apply(int) 
road_data.channel = road_data.channel.apply(int) 
road_data._from = road_data._from.apply(int) 
road_data.to = road_data.to.apply(int) 
road_data.isDuplex = road_data.isDuplex.apply(int) 


#    graph_data_temp = pd.concat([road_data._from,road_data.to,road_data.length, road_data.isDuplex], axis = 1)
graph_data_temp = pd.concat([road_data._from,road_data.to,road_data.length, road_data.speed,road_data.isDuplex], axis = 1)
graph_data = pd.DataFrame(index = np.arange(1,65),columns = np.arange(1,65))
  
for i in range(len(graph_data_temp)):
    x = graph_data_temp.iloc[i]._from
    y = graph_data_temp.iloc[i].to
    value = graph_data_temp.iloc[i].length/graph_data_temp.iloc[i].speed
    graph_data.loc[x,y] = value
    if(int(graph_data_temp.iloc[i].isDuplex)):  
        graph_data.loc[y,x] = value
    else:
        graph_data.loc[y,x] = float('inf')    
        
for i in range(1,65):
    graph_data.loc[i][i] = 0
    
graph_data = graph_data.fillna(float('inf'))

graph_list = np.array(graph_data)
arcs_high = graph_list  #车速快时，由道路限速限制通过时间   

graph_data = pd.DataFrame(index = np.arange(1,65),columns = np.arange(1,65))
for i in range(len(graph_data_temp)):
    x = graph_data_temp.iloc[i]._from
    y = graph_data_temp.iloc[i].to
    value = graph_data_temp.iloc[i].length
    graph_data.loc[x,y] = value
    if(int(graph_data_temp.iloc[i].isDuplex)):  
        graph_data.loc[y,x] = value
    else:
        graph_data.loc[y,x] = float('inf')    
for i in range(1,65):
    graph_data.loc[i][i] = 0  
graph_data = graph_data.fillna(float('inf'))
graph_list = np.array(graph_data)
arcs_low = graph_list  #车速慢时，因为达不到道路限速，所以由道路长度决定通过时间

d = [0]*64
fina = [0]*64
n = 64
     
answer = []
f = open(answer_path,'w+')
f.truncate()
#速度快的优先出发
#car_data = car_data.sort_values(by = ['speed','planTime'],ascending = (False,True))
for i in car_data.index:
    car_speed = car_data.loc[i].speed
    if car_speed <= 4:
        arcs = arcs_low
    else:
        arcs = arcs_high
    v0 = car_data.loc[i].start-1
    v1 = car_data.loc[i].to-1
    car_id = i
    planTime = car_data.loc[i].planTime
    low_add = 0 if car_speed == 8 else (125 if car_speed == 6 else (250 if car_speed == 4 else 375))
    high_add = 125 if car_speed == 8 else (250 if car_speed == 6 else (375 if car_speed == 4 else 500))
           
    planTime = int(int(planTime)+ np.random.uniform(low_add,high_add))
    answer_road = []
    p =[-1]*64
    dijkstra()
    path = Print(v0,n)
    answer_road.append('('+str(car_id))
    answer_road.append(planTime)
    path[-1] = str(path[-1])+')'
    answer_road = answer_road + path
    s =str(answer_road).replace('[','').replace(']','').replace("'",'') + '\n'
    f.write(s)
f.close()

