package scheduleSystem;

import pojo.CrossInschedule;

public interface ScheduleCross {
    /**
     * 调度一个路口
     */
    void scheduleOneCross(CrossInschedule crossInschedule);
    /**
     * 调度所有的路口
     */
    void scheduleAllCross();
}
