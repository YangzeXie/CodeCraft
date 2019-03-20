package scheduleSystem;

import pojo.Lane;
import pojo.RoadInschedule;
import java.util.List;

public interface ScheduleRoad {
    void updateOne(Lane lane);
    void updateOneRoad(RoadInschedule road);

    /**
     * updateAll应当返回每个路口上等待车辆的排队信息
     */
    void updateAll();
    void updateAll(List<RoadInschedule> roads);
}
