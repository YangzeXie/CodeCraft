package pojo;

import java.util.ArrayList;
import java.util.List;

public class CrossInschedule implements Comparable {
    private int id;
    private List<Integer> roadIndexPQ = new ArrayList<>();    //按优先级保存的每条路的下标, list[0]保存的是
                                                              // 优先级最高的那条路的下标
    private List<Integer> roadIds = new ArrayList<>(); //按逆时针顺序存储的id数据

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public List<Integer> getRoadIndexPQ() {
        return roadIndexPQ;
    }

    public void setRoadIndexPQ(List<Integer> roadIndexPQ) {
        this.roadIndexPQ = roadIndexPQ;
    }

    public List<Integer> getRoadIds() {
        return roadIds;
    }

    public void setRoadIds(List<Integer> roadIds) {
        this.roadIds = roadIds;
    }

    @Override
    public int compareTo(Object o) {
        CrossInschedule cross = (CrossInschedule) o;
        if (this.id < cross.id)
            return -1;
        if (this.id > cross.id)
            return 1;
        return 0;
    }
}
