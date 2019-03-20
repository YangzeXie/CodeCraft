package pojo;

import java.util.ArrayList;
import java.util.Deque;
import java.util.List;

public class RoadInschedule implements Comparable{
    private int id;
    private List<Lane> lanes = new ArrayList<>();
    private boolean isDone = false;
    private int speedLimit; //限速

    public void updateFirst(CarInschedule car) {
        int i = car.getLaneid();
        Deque<CarInschedule> cars = lanes.get(i).getCars();
        cars.removeFirst();
        cars.addFirst(car);
        lanes.get(i).setCars(cars);
    }
    public void updateLast(CarInschedule car) {
        int i = car.getLaneid();
        Deque<CarInschedule> cars = lanes.get(i).getCars();
        cars.removeLast();
        cars.addLast(car);
        lanes.get(i).setCars(cars);
    }

    public void remove(CarInschedule car) {
        int i = car.getLaneid();
        Deque<CarInschedule> cars = lanes.get(i).getCars();
        cars.removeFirst();
        lanes.get(i).setCars(cars);
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public boolean isDone() {
        return isDone;
    }

    public void setDone(boolean done) {
        isDone = done;
    }

    public List<Lane> getLanes() {
        return lanes;
    }

    public void setLanes(List<Lane> lanes) {
        this.lanes = lanes;
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
