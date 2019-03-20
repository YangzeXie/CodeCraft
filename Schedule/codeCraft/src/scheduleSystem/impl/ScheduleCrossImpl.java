package scheduleSystem.impl;

import pojo.*;
import scheduleSystem.ScheduleCross;

import java.util.*;

public class ScheduleCrossImpl implements ScheduleCross {
    //PriorityQueue<RoadInschedule> roadsInCross = new PriorityQueue<>();
    PriorityQueue<CrossInschedule> crosses = new PriorityQueue<>();
    Map<Integer, RoadInschedule> roads;
    /**
     * 调度器的构造函数，初始化crosses和roads,为调度做准备。
     * CrossInschedule放到PriorityQueue croses中
     * @param list
     */
    public ScheduleCrossImpl(List<Cross> list, Map<Integer, RoadInschedule> roads) {
        this.roads = roads;
        for (Cross cross : list) {
            CrossInschedule crossInschedule = new CrossInschedule();
            crossInschedule.setId(cross.getId());   //通过cross来设置id
            //设置每个cross的idlist(道路的id集合)
            List idlist = getRoadIdList(cross);
            crossInschedule.setRoadids(idlist);
            //设置每个cross的道路list
            //设置每个cross的道路优先队列
            List<RoadInschedule> roadsPQ = new ArrayList<>();   //roadsPQ保存此cross的路
            List roadList = new ArrayList();    //roadList保存此cross的路
            List<Integer> roadIdlist = cross.getRoadIdList();
            for (int i=0; i<4; i++) {
                if (roadIdlist.get(i) > -1){
                    roadsPQ.add(this.roads.get(roadIdlist.get(i)));
                } else if (roadIdlist.get(i) == -1) {
                    roadList.add(null);    //如果没有这条路则为null
                }
            }
            crossInschedule.setRoadsPQ(roadsPQ);
            crossInschedule.setRoadsList(roadList);
            crosses.add(crossInschedule);
        }
    }

    @Override
    public void scheduleOneCross(CrossInschedule crossInschedule) {
        List<RoadInschedule> roadsPQ = crossInschedule.getRoadsPQ();
        Collections.sort(roadsPQ);
        //对于每条路(按优先级选取)
        for (int i=0; i<roadsPQ.size(); i++) {
            RoadInschedule road = roadsPQ.get(i);
            //选择一辆在排队的车
            CarInschedule car = getCarFromRoad(road);
            //如果car为空，或者冲突了，跳过这条路
            if (car == null || isConflicted(crossInschedule, road, car))
                continue;   //这里有bug隐患，四条路都为空/冲突如何跳出循环
            int roadid = car.getRoadid();
            //如果没有冲突，则将该车调度一个时间片的距离
            //1.判断是否该车是否可以通过下一个车道
            if (!car.isCanOutCross()) {
                car.setLocation(car.getLocation() + car.getRealspeed());
                road.updateFirst(car);
                crossInschedule.update(road);
                continue;
            }
            //2.选择车道
            int ilane = choiceLane(road);
            if (ilane == -1)
                continue;
            //3.计算在下一个车道可以行进的距离
            int s2 = calcNextRoadMaxDistance(crossInschedule, car);
            //4.更新car，更新road，旧road里面的car要删除，新road添加
            RoadInschedule nextRoad = getNextRoad(crossInschedule, car);
            int nextroadid = roadsPQ.indexOf(nextRoad);
            car.setLocation(s2);
            car.setRoadspeedlimit(nextRoad.getSpeedLimit());
            car.setRealspeed(Math.min(car.getRoadspeedlimit(), car.getRoadspeedlimit()));
            road.remove(car);   //删除旧road里面的car
            car.setLaneid(ilane);
            //更新road？？
            nextRoad.updateLast(car);
            //5.更新roadsPQ
            roadsPQ.remove(nextRoad);
            roadsPQ.set(i, road);
            roadsPQ.set(nextroadid, nextRoad);
        }
        //更新cross
        crossInschedule.setRoadsPQ(roadsPQ);
        //crossInschedule.setRoadsList();
    }

    @Override
    public void scheduleAllCross() {
        PriorityQueue<CrossInschedule> crosses1 = new PriorityQueue<>();
        while (crosses.isEmpty()) {
            CrossInschedule cross = crosses.remove();
            scheduleOneCross(cross);
            crosses1.add(cross);
        }
        this.crosses = crosses1;
    }

    /**
     *
     * @param cross 当前的交叉路口
     * @param road  当前选取车辆的路口
     * @param car   当前选取的车
     * @return      返回是否冲突，boolean
     */
    private boolean isConflicted(CrossInschedule cross, RoadInschedule road, CarInschedule car) {
        List roadids = cross.getRoadids();
        //直行
        if (car.getDirection() == 3)
            return false;
        int i = roadids.indexOf(road.getId());
        //左转
        if (car.getDirection() == 2){
            //直行冲突检查
            if (i == 0) {
                CarInschedule car1 = getCarFromRoad(cross.getRoadsList().get(i + 3));
                if (car1 != null || car1.getDirection() == 3)
                    return false;
                else
                    return true;
            } else {
                CarInschedule car1 = getCarFromRoad(cross.getRoadsList().get(i - 1));
                if (car1 != null || car1.getDirection() == 3)
                    return false;
                else
                    return true;
            }
        } else {    //右转
            //直行冲突检查
            if (i == 4) {
                CarInschedule car1 = getCarFromRoad(cross.getRoadsList().get(0));
                if (car1 != null || car1.getDirection() == 3)
                    return false;
            } else {
                CarInschedule car1 = getCarFromRoad(cross.getRoadsList().get(i + 1));
                if (car1 != null || car1.getDirection() == 3)
                    return false;
            }
            //左行冲突检查
            if (i == 0 || i == 1) {
                CarInschedule car1 = getCarFromRoad(cross.getRoadsList().get(i + 2));
                if (car1 != null || car1.getDirection() == 3)
                    return false;
                else
                    return true;
            } else if (i == 2) {
                CarInschedule car1 = getCarFromRoad(cross.getRoadsList().get(0));
                if (car1 != null || car1.getDirection() == 3)
                    return false;
                else
                    return true;
            } else if (i==3) {
                CarInschedule car1 = getCarFromRoad(cross.getRoadsList().get(1));
                if (car1 != null || car1.getDirection() == 3)
                    return false;
                else
                    return true;
            }
        }
        return false;
    }

    /**
     * 选一辆在等待车，无论是否与其他车发生冲突。
     * 如果前road没有满足条件的等待车辆则返回null。
     * @param road
     * @return
     */
    private CarInschedule getCarFromRoad(RoadInschedule road) {
        for (Lane lane : road.getLanes()) {
            Deque<CarInschedule> cars = lane.getCars();
            for (CarInschedule car : cars) {
                if (car.isCanOutCross())
                    return car;
            }
        }
        return null;
    }

    private int choiceLane(RoadInschedule road) {
        List<Lane> lanes = road.getLanes();
        int i = 0;
        for (; i<lanes.size(); i++) {
            Deque<CarInschedule> cars = lanes.get(i).getCars();
            CarInschedule last = cars.getLast();
            if (last.getLocation() > 0)
                return i;
        }
        return -1;
    }

    private int calcNextRoadMaxDistance(CrossInschedule cross, CarInschedule car) {
        //通过car得到下一条路
        int nextroadid = car.getNextroadid();
        RoadInschedule nextroadInschedule = getNextRoad(cross, car);
        int nextspeedLimit = nextroadInschedule.getSpeedLimit();
        int v2 = Math.min(car.getSpeedlimit(), nextspeedLimit);
        int s1 = car.getDistance() - car.getLocation();
        int s2 = v2 > s1 ? v2 - s1 : 0;
        return s2;
    }

    private RoadInschedule getNextRoad (CrossInschedule cross, CarInschedule car) {
        //通过car得到下一条路
        int nextroadid = car.getNextroadid();
        List roadids = cross.getRoadids();
        int i = roadids.indexOf(nextroadid);
        RoadInschedule nextroadInschedule = cross.getRoadsList().get(i);
        return nextroadInschedule;
    }

    private RoadInschedule getNextRoad (CrossInschedule cross, int nextroadid) {
        //通过car得到下一条路
        List roadids = cross.getRoadids();
        int i = roadids.indexOf(nextroadid);
        RoadInschedule nextroadInschedule = cross.getRoadsList().get(i);
        return nextroadInschedule;
    }

    /**
     *
     * @param cross
     * @param
     * @return lane
     */
//    private RoadInschedule choiceAndUpdateRoad (CrossInschedule cross, CarInschedule car) {
//        RoadInschedule road = null;
//        //通过car得到下一条路
//        int nextroadid = car.getNextroadid();
//        List roadids = cross.getRoadids();
//        int i = roadids.indexOf(nextroadid);
//        RoadInschedule roadInschedule = cross.getRoadsList().get(i);
//
//        //依次判断每个lane是否有合适的位置
//        List<Lane> lanes = roadInschedule.getLanes();
//        for (Lane l : lanes) {
//            Deque<CarInschedule> cars = l.getCars();
//            CarInschedule last = cars.getLast();
//            //如果该车道有位置
//            if (last.getLocation() > 1) {
//
//            }
//        }
//        return null;
//    }



    public List getRoadIdList(Cross cross) {
        List idlist = new ArrayList();
        idlist.add(cross.getRoadId1());
        idlist.add(cross.getRoadId2());
        idlist.add(cross.getRoadId3());
        idlist.add(cross.getRoadId4());
        return idlist;
    }
}
