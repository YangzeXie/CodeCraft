package pojo;

public class Road {
    private int id; //id
    private int length; //
    private int speedLimit; //最高限速
    private int laneNums;   //车道数目
    private int beginId;    //起始点id
    private int endId;      //终点id
    private boolean isBidirectional;    //是否双向
    static public final int numberPara = 7;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getLength() {
        return length;
    }

    public void setLength(int length) {
        this.length = length;
    }

    public int getSpeedLimit() {
        return speedLimit;
    }

    public void setSpeedLimit(int speedLimit) {
        this.speedLimit = speedLimit;
    }

    public int getLaneNums() {
        return laneNums;
    }

    public void setLaneNums(int laneNums) {
        this.laneNums = laneNums;
    }

    public int getBeginId() {
        return beginId;
    }

    public void setBeginId(int beginId) {
        this.beginId = beginId;
    }

    public int getEndId() {
        return endId;
    }

    public void setEndId(int endId) {
        this.endId = endId;
    }

    public boolean isBidirectional() {
        return isBidirectional;
    }

    public void setBidirectional(boolean bidirectional) {
        isBidirectional = bidirectional;
    }

    @Override
    public String toString() {
        return "Road{" +
                "id=" + id +
                ", length=" + length +
                ", speedLimit=" + speedLimit +
                ", laneNums=" + laneNums +
                ", beginId=" + beginId +
                ", endId=" + endId +
                ", isBidirectional=" + isBidirectional +
                '}';
    }
}
