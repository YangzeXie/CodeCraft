package pojo;

public class Car {
    private int id;    //id
    private int beginId;    //始发地
    private int endId;      //目的地
    private int speedLimit;    //最高速度
    private int departTime;    //出发时间
    static public final int numberPara = 5;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
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

    public int getSpeedLimit() {
        return speedLimit;
    }

    public void setSpeedLimit(int speedLimit) {
        this.speedLimit = speedLimit;
    }

    public int getDepartTime() {
        return departTime;
    }

    public void setDepartTime(int departTime) {
        this.departTime = departTime;
    }

    @Override
    public String toString() {
        return "Car{" +
                "id=" + id +
                ", beginId=" + beginId +
                ", endId=" + endId +
                ", speedLimit=" + speedLimit +
                ", departTime=" + departTime +
                '}';
    }
}
