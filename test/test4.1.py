# -*- coding: utf-8 -*-
"""
Created on Sun Mar 17 10:58:51 2019

@author: BZK
"""
'''
使用道路通过时间替代了路径长度的dijkstra算法
'''
import pandas as pd
import numpy as np
import time

starttime = time.clock()

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
   

if __name__ == '__main__':
    
    car_data = pd.read_csv('car.txt',sep = ',',index_col=False,header=0)
    car_data.columns = ['id','start','to','speed','planTime']
    car_data = car_data.rename(columns=lambda x: x.replace("(","").replace(')','').replace('#',''))
    car_data.id = car_data.id.apply(lambda x: x.replace("(","").replace(')',''))
    car_data.planTime = car_data.planTime.apply(lambda x: x.replace(')',''))
    car_data.set_index(['id'], inplace =True)
    
    
    road_data = pd.read_csv('road.txt',sep = ',',index_col=False,header=0)
    road_data.columns = ['id','length','speed','channel','_from', 'to', 'isDuplex']
    road_data = road_data.rename(columns=lambda x: x.replace("(","").replace(')','').replace('#',''))
    road_data.id = road_data.id.apply(lambda x: x.replace("(","").replace(')',''))
    road_data.isDuplex = road_data.isDuplex.apply(lambda x: x.replace(')',''))
    
#    graph_data_temp = pd.concat([road_data._from,road_data.to,road_data.length, road_data.isDuplex], axis = 1)
    graph_data_temp = pd.concat([road_data._from,road_data.to,road_data.length, road_data.speed,road_data.isDuplex], axis = 1)
    graph_data = pd.DataFrame(index = np.arange(1,65),columns = np.arange(1,65))
  
    for i in range(len(graph_data_temp)):
        x = graph_data_temp.at[i,'_from']
        y = graph_data_temp.at[i,'to']
#        value = graph_data_temp.iloc[i].length
        value = graph_data_temp.at[i,'length']/graph_data_temp.iloc[i].speed
        graph_data.loc[x,y] = value
        if(int(graph_data_temp.at[i,'isDuplex'])):  
            graph_data.loc[y,x] = value
        else:
            graph_data.loc[y,x] = float('inf')    
    graph_data = graph_data.fillna(float('inf'))
    for i in range(1,65):
        graph_data.loc[i][i] = 0    
    graph_list = np.array(graph_data)
    arcs_high = graph_list  #车速快时，由道路限速限制通过时间

    graph_data = pd.DataFrame(index = np.arange(1,65),columns = np.arange(1,65))
    
    for i in range(len(graph_data_temp)):
        x = graph_data_temp.at[i,'_from']
        y = graph_data_temp.at[i,'to']
        value = graph_data_temp.iloc[i].length
#        value = graph_data_temp.iloc[i].length/graph_data_temp.iloc[i].speed
        graph_data.loc[x,y] = value
        if(int(graph_data_temp.at[i,'isDuplex'])):  
            graph_data.loc[y,x] = value
        else:
            graph_data.loc[y,x] = float('inf')    
    graph_data = graph_data.fillna(float('inf'))
    for i in range(1,65):
        graph_data.loc[i][i] = 0  
    graph_list = np.array(graph_data)
    arcs_low = graph_list  #车速慢时，因为达不到道路限速，所以由道路长度决定通过时间    
   
    d = [0]*64
    fina = [0]*64
    n = 64
        
#    v0 = 23
#    v1 = 50
    
#    answer = []
#    i = '10000'
#    v0 = car_data.loc[i].start-1
#    v1 = car_data.loc[i].to-1
#    car_id = i
#    planTime = car_data.loc[i].planTime
#    planTime = int(int(planTime)+ np.random.uniform(0,1000))
#    answer_road = []
#    p =[-1]*64
#    arcs = Map(graph_data,i)
#    dijkstra()
#    path = Print(v0,n)
#    answer_road.append('('+car_id)
#    answer_road.append(planTime)
#    path[-1] = str(path[-1])+')'
#    answer_road = answer_road + path
#    s =str(answer_road).replace('[','').replace(']','').replace("'",'') + '\n'   
    answer = []
    f = open('./answer.txt','a')
    f.truncate()
    for i in car_data.index:
        if car_data.loc[i].speed <= 4:
            arcs = arcs_low
        else:
            arcs = arcs_high
        v0 = car_data.loc[i].start-1
        v1 = car_data.loc[i].to-1
        car_id = i
        planTime = car_data.loc[i].planTime
        planTime = int(int(planTime)+ np.random.uniform(0,1000))
        answer_road = []
        p =[-1]*64
        dijkstra()
        path = Print(v0,n)
        answer_road.append('('+car_id)
        answer_road.append(planTime)
        path[-1] = str(path[-1])+')'
        answer_road = answer_road + path
        s =str(answer_road).replace('[','').replace(']','').replace("'",'') + '\n'
        f.write(s)
    f.close()

endtime = time.clock()
print ('运行时间为：',(endtime - starttime),' 秒')
