package pojo;

import java.util.ArrayList;
import java.util.List;

public class CrossInschedule implements Comparable{
    private int id;
    private List<Integer> roadids = new ArrayList<>();
    private List<RoadInschedule> roadsPQ = new ArrayList<>();   //实际上未必有序，每次要用时应当先排序
    private List<RoadInschedule> roadsList = new ArrayList<>(); //按逆时针顺序存储的数据

    public void update (RoadInschedule road) {
        for (RoadInschedule rd : roadsPQ) {
            if (rd.getId() == road.getId()) {
                roadsPQ.remove(rd);
                roadsPQ.add(road);
            }
        }
        for (int i=0; i<roadsList.size(); i++) {
            if (road.getId() == roadsList.get(i).getId())
                roadsList.set(i, road);
        }

    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public List<RoadInschedule> getRoadsPQ() {
        return roadsPQ;
    }

    public List<RoadInschedule> getRoadsList() {
        return roadsList;
    }

    public void setRoadsList(List<RoadInschedule> roadsList) {
        this.roadsList = roadsList;
    }

    public void setRoadsPQ(List<RoadInschedule> roadsInCross) {
        this.roadsPQ = roadsInCross;
    }

    public List getRoadids() {
        return roadids;
    }

    public void setRoadids(List roadids) {
        this.roadids = roadids;
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
