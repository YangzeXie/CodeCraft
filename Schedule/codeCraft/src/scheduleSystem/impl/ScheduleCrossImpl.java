package scheduleSystem.impl;

import pojo.*;
import scheduleSystem.ScheduleCross;

import java.util.*;

public class ScheduleCrossImpl implements ScheduleCross {
    Map<Integer, RoadInschedule> roads;
    Map<Integer, CrossInschedule> crosses = new HashMap<>();

    public ScheduleCrossImpl(List<Cross> list, Map<Integer, RoadInschedule> roads) {
        this.roads = roads;
        for (Cross cross : list) {
            CrossInschedule crossInschedule = new CrossInschedule();
            crossInschedule.setId(cross.getId());   //通过cross来设置id
            //设置每个cross的roadIds(道路的id集合/按逆时针顺序)
            List<Integer> roadIds = getRoadIdList(cross);
            crossInschedule.setRoadIds(roadIds);
            //设置每个cross的道路roadIndexPQ
            List<Integer> roadIndexPQ = getRoadIndexPQ(cross);
            crossInschedule.setRoadIndexPQ(roadIndexPQ);
            crosses.put(crossInschedule.getId(), crossInschedule);
        }
        this.roads = roads;
    }

    public List getRoadIdList(Cross cross) {
        List idlist = new ArrayList();
        idlist.add(cross.getRoadId1());
        idlist.add(cross.getRoadId2());
        idlist.add(cross.getRoadId3());
        idlist.add(cross.getRoadId4());
        return idlist;
    }

    public List getRoadIndexPQ(Cross cross) {
        List<Integer> roadIndexPQ = new ArrayList();
        List<Integer> ids = getRoadIdList(cross);
        List<Integer> idstem = getRoadIdList(cross);
        Collections.sort(idstem);
        for (int i=0; i<idstem.size(); i++) {
            if (idstem.get(i) < 0)
                continue;
            int i1 = ids.indexOf(idstem.get(i));
            roadIndexPQ.add(i1);
        }
        return roadIndexPQ;
    }

    /**
     * 选一辆在等待车，无论是否与其他车发生冲突。
     * 如果前road没有满足条件的等待车辆则返回null。
     * @param road
     * @return
     */
    private CarInschedule getCarFromRoad(CrossInschedule cross, RoadInschedule road) {
        List<Lane> lanes = getLanes(cross, road);
        for (Lane lane : lanes) {
            Deque<CarInschedule> cars = lane.getCars();
            for (CarInschedule car : cars) {
                if (car.isWaitflag())
                    return car;
            }
        }
        return null;
    }
    
    private List<Lane> getLanes(CrossInschedule cross, RoadInschedule road) {
        int beginId = road.getBeginId();
        int endId = road.getEndId();
        String fromTo = cross.getId() == beginId ? endId + "->" + beginId : beginId + "->" + endId;
        Map<String, List<Lane>> lanemap = road.getLanemap();
        List<Lane>  lanes = lanemap.get(fromTo);
        return lanes;
    }

    @Override
    public void scheduleOneCross(CrossInschedule crossInschedule) {
        while (shceduleOneCrossOneRound(crossInschedule));
    }

    private boolean shceduleOneCrossOneRound(CrossInschedule crossInschedule) {
        List<Integer> roadsIndexPQ = crossInschedule.getRoadIndexPQ();
        List<Integer> roadIds = crossInschedule.getRoadIds();
        int[] flags = new int[4];
        //对于每条路(按优先级选取)
        for (int i=0; i<roadsIndexPQ.size(); i++) {
            if (i%4==0) {
                flags = new int[4];
            }
            Integer index = roadsIndexPQ.get(i);
            int roadid = roadIds.get(index);
            if (roadid == -1)   //如果此位置没有连接道路
                continue;
            RoadInschedule road = roads.get(roadid);
            //选择一辆在排队的车
            CarInschedule car = getCarFromRoad(crossInschedule, road);
            //如果car为空，或者冲突了，跳过这条路
            if (car == null || isConflicted(crossInschedule, road, car, i))
                continue;   //这里有bug隐患，四条路都为空/冲突如何跳出循环
            //如果没有冲突，则将该车调度一个时间片的距离
            //1.判断是否该车是否可以通过下一个车道
            if (!car.isCanOutCross()) {
                car.setLocation(car.getLocation() + car.getRealspeed());
                road.updateFirst(car);
                //更新map roads
                roads.put(road.getId(), road);
                flags[index] = 1;   //index这条路动了
                continue;
            }
            //2.选择车道
            int nextroadid = car.getNextroadid();
            RoadInschedule nextroad = roads.get(nextroadid);
            int ilane = choiceLane(crossInschedule, nextroad);
            if (ilane == -1)
                continue;
            flags[index] = 1;   //index这条路动了
            //3.计算在下一个车道可以行进的距离
            int s2 = calcNextRoadMaxDistance(crossInschedule, car);
            //4.更新car，更新road，旧road里面的car要删除，新road添加
            int oldlaneid = car.getLaneid();    //保存旧的lane的id
            RoadInschedule nextRoad = roads.get(nextroadid);
            car.setRoadid(nextroadid);
            car.setLocation(s2);
            car.setRoadspeedlimit(nextRoad.getSpeedLimit());
            car.setRealspeed(Math.min(car.getRoadspeedlimit(), car.getRoadspeedlimit()));
            car.setLaneid(ilane);
            car.setWaitflag(false);
            car.setStopflag(true);
            car.setDistance(nextroad.getLength());
            car.setCanOutCross(false);
            int nextcrossid = (crossInschedule.getId() == nextroad.getBeginId() ?
                    nextroad.getEndId() : nextroad.getBeginId());
            car.setFromTo(crossInschedule.getId()+""+nextcrossid);
            //car.setNextroadid();  //当前无法判断需要Schedule和路径规划判断。
            //car.setDirection();   //当前无法判断需要Schedule和路径规划判断。
            road.removeFirst(car);   //删除旧road里面的car
            nextRoad.updateLast(car);
            //5. 需要调度/更新一下该lane
            ScheduleRoadImpl scheduleRoad = new ScheduleRoadImpl(this.roads);
            int lastcorssid = (crossInschedule.getId() == road.getBeginId() ? road.getEndId():road.getBeginId());
            scheduleRoad.updateOne(road, oldlaneid, lastcorssid+""+crossInschedule.getId());
            nextRoad.updateLast(car);

            //6. 最后需要更新roads
            roads.put(road.getId(), road);
            roads.put(nextroadid, nextRoad);

            //对每一轮进行判断，如果只要有路动了，就认为这次调度是可以继续的
            if (i%4==3){
                return flags[0]==1 || flags[1]==1 || flags[2]==1 || flags[3]==1;
            }
        }
        return false;
    }

    private int calcNextRoadMaxDistance(CrossInschedule cross, CarInschedule car) {
        //通过car得到下一条路
        int nextroadid = car.getNextroadid();
        RoadInschedule nextroadInschedule = roads.get(nextroadid);
        int nextspeedLimit = nextroadInschedule.getSpeedLimit();
        int v2 = Math.min(car.getSpeedlimit(), nextspeedLimit);
        int s1 = car.getDistance() - car.getLocation();
        int s2 = v2 > s1 ? v2 - s1 : 0;
        return s2;
    }

    private int choiceLane(CrossInschedule cross, RoadInschedule road) {
        List<Lane> lanes = getLanes(cross, road);
        int i = 0;
        for (; i<lanes.size(); i++) {
            Deque<CarInschedule> cars = lanes.get(i).getCars();
            CarInschedule last = cars.getLast();
            if (last.getLocation() > 0)
                return i;
        }
        return -1;
    }

    private boolean isConflicted(CrossInschedule cross, RoadInschedule road, CarInschedule car, int i) {
        List roadids = cross.getRoadIds();
        //直行
        if (car.getDirection() == 3)
            return false;
        CarInschedule car1;
        //左转
        if (car.getDirection() == 2){
            //直行冲突检查
            if (i == 0)
                car1 = getCarFromRoad(cross, roads.get(roadids.get(i+3)));
            else
                car1 = getCarFromRoad(cross, roads.get(roadids.get(i-1)));
            return  car!=null && car1.getDirection()==3;
        } else {    //右转
            //直行冲突检查
            if (i == 4)
                car1 = getCarFromRoad(cross, roads.get(roadids.get(0)));
            else
                car1 = getCarFromRoad(cross, roads.get(roadids.get(i+1)));
            //左行冲突检查
            if (i == 0 || i == 1)
                car1 = getCarFromRoad(cross, roads.get(roadids.get(i+2)));
            else if (i == 2)
                car1 = getCarFromRoad(cross, roads.get(roadids.get(0)));
            else if (i==3)
                car1 = getCarFromRoad(cross, roads.get(roadids.get(1)));
            return (car1 != null && car1.getDirection() == 2) || (car1 != null && car1.getDirection() == 3);
        }
    }

    @Override
    public void scheduleAllCross() {
        //按优先级更新各个cross
        for (Map.Entry<Integer, CrossInschedule> entry : crosses.entrySet()) {
            scheduleOneCross(entry.getValue());
        }
    }
}
