package scheduleSystem;

import org.junit.jupiter.api.Test;
import pojo.CarInschedule;
import pojo.Lane;
import scheduleSystem.impl.ScheduleRoadImpl;

import java.util.Deque;
import java.util.Iterator;
import java.util.LinkedList;

class ScheduleRoadImplTest {

    @Test
    void updateOne() {
        Lane lane = new Lane();
        Deque<CarInschedule> cars = new LinkedList<>();

        ScheduleRoad sr = new ScheduleRoadImpl();
        sr.updateOne(lane);

        Deque<Integer> integerDeque = new LinkedList<>();
        integerDeque.addFirst(1);
        integerDeque.addFirst(2);
        integerDeque.addFirst(3);
        Iterator<Integer> it = integerDeque.iterator();
        int n = it.next();
        System.out.println(n);
    }
}