package scheduleSystem;

import pojo.Car;
import pojo.Cross;
import pojo.RoadInschedule;

import java.util.List;
import java.util.Map;

public interface Schedule {
    /**
     * 在车库里选择合适的车出发
     * @param carsInCarport
     * @param list
     */
    void choiceNewCarOnTORoad(List<Car> carsInCarport, List<RoadInschedule> list);
    /**
     * 调整所有道路上在道路上的车辆，让道路上车辆前进，只要不出路口且可以到达终止状态的车辆
     * 分别标记出来等待的车辆（要出路口的车辆，或者因为要出路口的车辆阻挡而不能前进的车辆）
     * 和终止状态的车辆（在该车道内可以经过这一次调度可以行驶其最大可行驶距离的车辆）
     * @param list
     * @return
     */
    Map<Integer, RoadInschedule> scheduleCarsOnRoads(List<RoadInschedule> list);

    /**
     * 调度所有在cross里面的车
     * @param list
     * @param roads
     * @return
     */
    List<RoadInschedule> scheduleCarsInCross(List<Cross> list, Map<Integer, RoadInschedule> roads);
}
