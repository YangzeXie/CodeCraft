package pojo;

import java.util.*;

public class RoadInschedule implements Comparable{
    private int id;
    //key:"roadid"+"nextroadid", value存的是lane的list
    private Map<String, List<Lane>> lanemap = new HashMap<>();
    private boolean isDone = false;
    private int speedLimit; //限速
    private int beginId;
    private int endId;
    private boolean isBidirectional;
    private int length;

    public void updateFirst(CarInschedule car) {
        int i = car.getLaneid();
        List<Lane> lanes = lanemap.get(car.getRoadid()+"->"+car.getNextroadid());
        Deque<CarInschedule> cars = lanes.get(i).getCars();
        cars.removeFirst();
        cars.addFirst(car);
        lanes.get(i).setCars(cars);
        lanemap.put(car.getRoadid()+"->"+car.getNextroadid(), lanes);
    }

    public void updateLast(CarInschedule car) {
        int i = car.getLaneid();
        List<Lane> lanes = lanemap.get(car.getRoadid()+"->"+car.getNextroadid());
        Deque<CarInschedule> cars = lanes.get(i).getCars();
        cars.removeLast();
        cars.addLast(car);
        lanes.get(i).setCars(cars);
        lanemap.put(car.getRoadid()+"->"+car.getNextroadid(), lanes);
    }

    public void removeFirst(CarInschedule car) {
        int i = car.getLaneid();
        List<Lane> lanes = lanemap.get(car.getRoadid()+"->"+car.getNextroadid());
        Deque<CarInschedule> cars = lanes.get(i).getCars();
        cars.removeFirst();
        lanes.get(i).setCars(cars);
        lanemap.put(car.getRoadid()+"->"+car.getNextroadid(), lanes);
    }

    public int getLength() {
        return length;
    }

    public void setLength(int length) {
        this.length = length;
    }

    public int getBeginId() {
        return beginId;
    }

    public void setBeginId(int beginId) {
        this.beginId = beginId;
    }

    public boolean isBidirectional() {
        return isBidirectional;
    }

    public void setBidirectional(boolean bidirectional) {
        isBidirectional = bidirectional;
    }

    public int getEndId() {
        return endId;
    }

    public void setEndId(int endId) {
        this.endId = endId;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public Map<String, List<Lane>> getLanemap() {
        return lanemap;
    }

    public void setLanemap(Map<String, List<Lane>> lanemap) {
        this.lanemap = lanemap;
    }

    public boolean isDone() {
        return isDone;
    }

    public void setDone(boolean done) {
        isDone = done;
    }

    public int getSpeedLimit() {
        return speedLimit;
    }

    public void setSpeedLimit(int speedLimit) {
        this.speedLimit = speedLimit;
    }

    @Override
    public int compareTo(Object o) {
        RoadInschedule object = (RoadInschedule) o;
        if (this.id < object.id)
            return -1;
        else if (this.id > object.id)
            return 1;
        else
            return 0;
    }
}
