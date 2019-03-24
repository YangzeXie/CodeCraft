package scheduleSystem.impl;

import pojo.Car;
import pojo.Cross;
import pojo.RoadInschedule;
import scheduleSystem.Schedule;

import java.util.List;
import java.util.Map;

public class ScheduleImpl implements Schedule {
    @Override
    public void choiceNewCarOnTORoad(List<Car> carsInCarport, List<RoadInschedule> list) {

    }

    @Override
    public Map<Integer, RoadInschedule> scheduleCarsOnRoads(List<RoadInschedule> list) {
        return null;
    }

    @Override
    public List<RoadInschedule> scheduleCarsInCross(List<Cross> list, Map<Integer, RoadInschedule> roads) {
        return null;
    }
}
