package pojo;

import java.util.Deque;
import java.util.LinkedList;

public class Lane {
    private Deque<CarInschedule> cars = new LinkedList<>();
    private boolean isDone = false;
    private int roadid;

    public boolean isDone() {
        return isDone;
    }

    public int getRoadid() {
        return roadid;
    }

    public void setRoadid(int roadid) {
        this.roadid = roadid;
    }

    public void setDone(boolean done) {
        isDone = done;
    }

    public Deque<CarInschedule> getCars() {
        return cars;
    }

    public void setCars(Deque<CarInschedule> cars) {
        this.cars = cars;
    }
}
