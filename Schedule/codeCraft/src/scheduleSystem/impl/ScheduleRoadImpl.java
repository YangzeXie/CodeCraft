package scheduleSystem.impl;

import pojo.CarInschedule;
import pojo.Lane;
import pojo.RoadInschedule;
import scheduleSystem.ScheduleRoad;
import java.util.*;

public class ScheduleRoadImpl implements ScheduleRoad {
    //key: road id, value: road
    Map<Integer, RoadInschedule> roads = new HashMap<>();

    public ScheduleRoadImpl(List<RoadInschedule> roads) {
        for (RoadInschedule road : roads) {
            this.roads.put(road.getId(), road);
        }
    }

    public ScheduleRoadImpl(Map<Integer, RoadInschedule> roads) {
        this.roads = roads;
    }

    public ScheduleRoadImpl() {

    }

    @Override
    public void updateOne(Lane lane) {
        Deque<CarInschedule> cars = lane.getCars();
        updateCars(cars);
        lane.setCars(cars);
    }

    public void updateOne(RoadInschedule road, int laneid, String fromTo) {
        Deque<CarInschedule> cars = road.getLanemap().get(fromTo).get(laneid).getCars();
        updateCars(cars);
        Map<String, List<Lane>> lanemap = road.getLanemap();
        Lane lane = new Lane();
        lane.setCars(cars);
        List<Lane> lanes = lanemap.get(fromTo);
        lanes.set(laneid, lane);
        lanemap.put(fromTo, lanes);
        road.setLanemap(lanemap);
    }

    private void updateCars(Deque<CarInschedule> cars) {
        Iterator<CarInschedule> it = cars.iterator();
        CarInschedule car;
        CarInschedule carLast = null;
        //对于第一辆车
        if (it.hasNext()) {
            car = it.next();
            setCarByInfo(car, null);
            carLast = car;
        }

        while (it.hasNext()) {
            car = it.next();
            setCarByInfo(car, carLast);
            carLast = car;
        }
    }

    private void setCarByInfo(CarInschedule car, CarInschedule carLast) {
        //判断是否是第一辆车(最靠近下一个路口的车)
        if (carLast==null) {
            //如果此时间片该车到达出路口位置
            if (car.getLocation() + car.getRealspeed() >= car.getDistance()) {
                car.setWaitflag(true);
                car.setLocation(car.getDistance());
                car.setCanOutCross(true);
            } else {    //否则更新location
                car.setLocation(car.getLocation()+car.getRealspeed());
                car.setStopflag(true);
                if (car.getLocation() + car.getRealspeed() > car.getDistance())
                    car.setCanOutCross(true);
            }
        } else {
            int s = carLast.getLocation() - car.getLocation();
            //如果前面一辆车处于等待状态
            if (carLast.isWaitflag()) {
                car.setWaitflag(true);  //更新状态
                //如果当前车速*时间大于s
                if (s < car.getRealspeed())
                    car.setLocation(carLast.getLocation());
                else
                    car.setLocation(car.getLocation() + car.getRealspeed());
            } else if (carLast.isStopflag()) {
                car.setStopflag(true);
                car.setRealspeed(Math.min(car.getRealspeed(), s));
                car.setLocation(car.getLocation()+car.getRealspeed());
            } else {
                car.setStopflag(true);
                car.setLocation(car.getLocation()+car.getRealspeed());
            }
        }
    }

    @Override
    public void updateOneRoad(RoadInschedule road, String fromTo) {
        List<Lane> lanes = road.getLanemap().get(fromTo);
        for (Lane lane : lanes) {
            updateOne(lane);
        }
    }

    @Override
    public void updateAll() {
        for (Map.Entry<Integer, RoadInschedule> entry : roads.entrySet()) {
            RoadInschedule road = entry.getValue();
            updateOneRoad(road, road.getBeginId()+"->"+road.getEndId());
            if (road.isBidirectional())
                updateOneRoad(road, road.getEndId()+"->"+road.getBeginId());
        }
    }

    @Override
    public void updateAll(List<RoadInschedule> roads) {
//        for (RoadInschedule road : roads) {
//            updateOneRoad(road);
//        }
    }

    public Map<Integer, RoadInschedule> getRoads() {
        return roads;
    }

    public void setRoads(Map<Integer, RoadInschedule> roads) {
        this.roads = roads;
    }
}
