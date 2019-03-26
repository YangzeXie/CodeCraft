//
// Created by Zhibo Zhou on 2019-03-17.
//

#ifndef CODECRAFT_2019_ELEMENT_H
#define CODECRAFT_2019_ELEMENT_H

#include <vector>
#include <iostream>

#define STRAIGHT 0
#define LEFT 1
#define RIGHT 2

class DynamicArray {
private:
    int row;
    int col;
    int **array2D;

public:
    DynamicArray(int a = 0, int b = 0) {
        row = a;
        col = b;
        array2D = new int *[row];
        if (array2D == NULL) {
            std::cout << "Allocation failure!\n";
            exit(15);
        }

        for (int i = 0; i < row; ++i) {
            array2D[i] = new int[col];
            if (array2D[i] == NULL) {
                std::cout << "Allocation failure!\n";
                exit(15);
            }
        }

        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                array2D[i][j] = -1;
            }
        }
    }

    DynamicArray(DynamicArray &arr) {
        row = arr.row;
        col = arr.col;
        array2D = new int *[row];
        if (array2D == NULL) {
            std::cout << "Allocation failure!\n";
            exit(15);
        }

        for (int i = 0; i < row; ++i) {
            array2D[i] = new int[col];
            if (array2D[i] == NULL) {
                std::cout << "Allocation failure!\n";
                exit(15);
            }
        }

        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                array2D[i][j] = arr.getValue(i, j);
            }
            std::cout << '\n';
        }
    }

    int getRowSize() {
        return row;
    }

    int getColSize() {
        return col;
    }

    void setValue(int x, int y, int value) {
        if (x < 0 || x > row - 1 || y < 0 || y > col - 1) {
            std::cout << "Error!" << std::endl;
            exit(16);
        }

        array2D[x][y] = value;
    }

    int getValue(int x, int y) {

        if (x < 0 || x > row - 1 || y < 0 || y > col - 1) {
            std::cout << "Error!" << std::endl;
            exit(16);
        }
        return array2D[x][y];
    }

    void displayArray() {
        for (int i = 0; i < row; ++i) {
            std::cout << '\t';
            for (int j = 0; j < col; ++j) {
                std::cout << array2D[i][j] << '\t';
            }
            std::cout << '\n';
        }
    }

    ~DynamicArray() {
        for (int i = 0; i < row; i++) {
            delete[] array2D[i];
        }
        delete[] array2D;
    };
};

struct nextMove {
    int crossID;
    int roadID;

};

class cross {
private:
    int ID;
    int NumOfRoads;
    int roadIDSet[4];
public:
    std::vector<int> sortedRoadIDList;
    std::vector<int> sortedInRoadIDList;
    std::vector<int> sortedOutRoadIDList;
    int NumOfInRoads;
    int NumOfOutRoads;
    int carNumInWaiting;

public:
    cross(int id, int r1, int r2, int r3, int r4) {
        ID = id;
        roadIDSet[0] = r1;
        roadIDSet[1] = r2;
        roadIDSet[2] = r3;
        roadIDSet[3] = r4;

        int tmp = 4;
        for (int i = 0; i < 4; ++i) {
            if (roadIDSet[i] < 0) {
                tmp--;
            }
        }
        NumOfRoads = tmp;
        NumOfInRoads = 0;
        NumOfOutRoads = 0;
        carNumInWaiting = 0;
    }

    int getCrossID() {
        return ID;
    }

    int getNumOfRoads() {
        return NumOfRoads;
    }

    int getRoadIDSet(int n) {
        return roadIDSet[n];
    }

    void updateCrossID(int firstID) {
        ID -= firstID;
    }

    void updateRoadID(int firstID) {
        for (int i = 0; i < 4; ++i) {
            if (roadIDSet[i] != -1) {
                roadIDSet[i] -= firstID;
            }
        }
    }


    void sortRoadID() {
        for (int i = 0; i < 4; ++i) {
            if (roadIDSet[i] != -1) {
                sortedRoadIDList.push_back(roadIDSet[i]);
            }
        }
        std::sort(sortedRoadIDList.begin(), sortedRoadIDList.end());

    }

};

class simplexRoad {
private:
    int NextLeftRoadID;
    int NextRightRoadID;
    int NextStraightRoadID;
    DynamicArray roadInfoArray;

public:
    simplexRoad(int a, int b) : roadInfoArray(a, b) {
        int arrayRowSize = roadInfoArray.getRowSize();
        int arrayColSize = roadInfoArray.getColSize();

        // 初始化为-1，表示暂时该格上无车。
        for (int i = 0; i < arrayRowSize; ++i) {
            for (int j = 0; j < arrayColSize; ++j) {
                roadInfoArray.setValue(i, j, -1);
            }
        }

        NextLeftRoadID = -1;
        NextRightRoadID = -1;
        NextStraightRoadID = -1;
    }

    simplexRoad(const simplexRoad &arr) {
        NextLeftRoadID = arr.NextLeftRoadID;
        NextRightRoadID = arr.NextRightRoadID;
        NextStraightRoadID = arr.NextStraightRoadID;
        roadInfoArray = arr.roadInfoArray;
    }

    void setNextRoadID(int direction, int roadId) {
        if (direction == STRAIGHT) {
            NextStraightRoadID = roadId;
        }
        if (direction == LEFT) {
            NextLeftRoadID = roadId;
        }
        if (direction == RIGHT) {
            NextRightRoadID = roadId;
        }
    }

    int getNextRoadID(int direction) {
        if (direction == STRAIGHT) {
            return NextStraightRoadID;
        } else if (direction == LEFT) {
            return NextLeftRoadID;
        } else if (direction == RIGHT) {
            return NextRightRoadID;
        } else {
            return -1;
        }
    }

    void setRoadInfo(int x, int y, int carId) {
        roadInfoArray.setValue(x, y, carId);
    }

    int getRoadInfo(int x, int y) {
        return roadInfoArray.getValue(x, y);
    }

    void display() {
        roadInfoArray.displayArray();
    }
};


class CarPlan {
public:
    int roadID;
    int orientation; // 0:+,1:-;
    int nextDirection;

};

class road {
private:
    int ID;
    int IsDuplex;
    int Length;
    int NumOfLane;
    int StartCrossID;
    int EndCrossID;
    int MaxSpeed;
    simplexRoad road0;
    simplexRoad road1;

public:
    road(int id, int isDuplex, int len, int laneNum, int startId, int endId, int maxspeed) : road0(laneNum, len),
                                                                                             road1(laneNum, len) {
        ID = id;
        IsDuplex = isDuplex;
        Length = len;
        NumOfLane = laneNum;
        StartCrossID = startId;
        EndCrossID = endId;
        MaxSpeed = maxspeed;
    }

    road(const road &arr) : road0(arr.NumOfLane, arr.Length), road1(arr.NumOfLane, arr.Length) {
        ID = arr.ID;
        IsDuplex = arr.IsDuplex;
        Length = arr.Length;
        NumOfLane = arr.NumOfLane;
        StartCrossID = arr.StartCrossID;
        EndCrossID = arr.EndCrossID;
        MaxSpeed = arr.MaxSpeed;
    }

    int getRoadMaxSpeed() {
        return MaxSpeed;
    }

    int getRoadID() {
        return ID;
    }

    int getStartCrossID() {
        return StartCrossID;
    }

    int getEndCrossID() {
        return EndCrossID;
    }

    int getRoadLength() {
        return Length;
    }

    int getLaneNum() {
        return NumOfLane;
    }

    void updateRoadID(int firstID) {
        ID -= firstID;
    }

    void updateCrossID(int firstID) {
        StartCrossID -= firstID;
        EndCrossID -= firstID;
    }

    int isDuplex() {
        return IsDuplex;
    }

    void setSimplexRoadNextRoadID(int thisRoadDirection, int nextRoadDirection, int roadID) {
        if (thisRoadDirection == 0) {
            road0.setNextRoadID(nextRoadDirection, roadID);
        }

        if (thisRoadDirection == 1) {
            road1.setNextRoadID(nextRoadDirection, roadID);
        }
    }

    int getSimplexRoadNextRoadID(int thisRoadDirection, int nextRoadDirection) {
        if (thisRoadDirection == 0) {
            return road0.getNextRoadID(nextRoadDirection);
        }

        if (thisRoadDirection == 1) {
            return road1.getNextRoadID(nextRoadDirection);
        }
    }

    int getSimplexRoadStartCrossID(int thisRoadDirection) {
        if (thisRoadDirection == 1) {
            return EndCrossID;
        }

        if (thisRoadDirection == 0) {
            return StartCrossID;
        }
    }

    int getSimplexRoadEndCrossID(int thisRoadDirection) {
        if (thisRoadDirection == 0) {
            return EndCrossID;
        }

        if (thisRoadDirection == 1) {
            return StartCrossID;
        }
    }

    void changeGridState(int direction, int x, int y, int carId) {

        if (!IsDuplex && direction == 1) {
            std::cout << "[ERROR] Road " << ID << " is simplex." << std::endl;
            exit(17);
        }

        if (direction == 0) //start --> end, road0;
        {
            road0.setRoadInfo(x, y, carId);
        }

        if (direction == 1) //end --> start, road1;
        {
            road1.setRoadInfo(x, y, carId);
        }
    }

    int getGridState(int direction, int x, int y) {

        if (!IsDuplex && direction == 1) {
            std::cout << "[ERROR] Road " << ID << " is simplex." << std::endl;
            exit(17);
        } else if (direction == 0) //start --> end, road0;
        {
            return road0.getRoadInfo(x, y);
        } else if (direction == 1) //end --> start, road1;
        {
            return road1.getRoadInfo(x, y);
        } else {
            return -999;
        }
    }

    void display() {
        if (IsDuplex) {
            std::cout << "[ROAD INFO] " << EndCrossID + 1 << " <<<< " << ID + 501 << " <<<< " << StartCrossID + 1
                      << " :" << std::endl;
            road0.display();
            std::cout << "[ROAD INFO] " << StartCrossID + 1 << " <<<< " << ID + 501 << " <<<< " << EndCrossID + 1
                      << " :" << std::endl;
            road1.display();
        } else {
            std::cout << "[ROAD INFO] " << EndCrossID + 1 << " <<<< " << ID + 501 << " <<<< " << StartCrossID + 1
                      << " :" << std::endl;
            road0.display();
        }
        std::cout << std::endl;
    }
};


class car {
public:
    int ID;
    int MaxCarSpeed;
    int StartPoint;
    int EndPoint;
    int MinTime;
    int PlannedTime;
    int ActualTime;
    int FinishedTime;
    int CarState; // -1:未出发；0:已完成调度(终止状态)；1:正在调度(等待状态)
    class CarPosition {
    public:
        int roadID;
        int orientation;
        int laneID;
        int positionID;
    public:
        CarPosition(int a = 0, int b = 0, int c = 0, int d = -1) : roadID(a), laneID(b), positionID(c),
                                                                   orientation(d) {}
    } presentPosition, expectedPosition;

    std::vector<CarPlan> Plan;
    int PlanStep;

public:
    car(int id = 0, int maxcarspeed = 0, int startpoint = 0, int endpoint = 0, int mintime = 0) :
            ID(id),
            MaxCarSpeed(maxcarspeed),
            StartPoint(startpoint),
            EndPoint(endpoint),
            MinTime(mintime) {
        CarState = -1;
        ActualTime = 0;
        PlannedTime = 0;
        PlanStep = -1;
        FinishedTime = 99999;

    }

    void pushCarPlan(CarPlan plan) {
        Plan.push_back(plan);
        PlanStep = 0;
    }

    void updateCarID(int firstID) {
        ID -= firstID;
    }

    void updateCrossID(int firstID) {
        StartPoint -= firstID;
        EndPoint -= firstID;
    }

    int getPresnetRoadID() {
        return presentPosition.roadID;
    }

    int getPresentRoadOri() {
        return presentPosition.orientation;
    }


    void displayPlan() {
        std::cout << "[CAR PLAN]" << '\t' << ID << '\n' << "Road_ID" << '\t' << "Orientation" << '\t'
                  << "Next_Direction" << '\n';
        for (int i = 0; i < Plan.size(); ++i) {
            std::cout << Plan[i].roadID + 501 << '\t' << Plan[i].orientation << '\t' << Plan[i].nextDirection << '\n';
        }
    }
};

class systemInfo {
public:
    int Time;
    int TotalCarNum;
    int OnlineCarNum;
    int OfflineCarNum;
    int FinishedCarNum;
    int carNumInWaiting;
    std::vector<int> OnlineCarList;
    std::vector<int> OfflineCarList;
    std::vector<int> FinishedCarList;
public:
    systemInfo() {
        Time = 0;
        OnlineCarNum = 0;
        FinishedCarNum = 0;
        carNumInWaiting = 0;
    }

    void inputCarVector(std::vector<int> caridvector) {
        OfflineCarList = caridvector;
        OfflineCarNum = OfflineCarList.size();
    }

    int getOnlineCarNum() {
        return OnlineCarList.size();
    }

    int getOfflineCarNum() {
        return OfflineCarList.size();
    }

    int getFinishedCarNum() {
        return FinishedCarList.size();
    }

    void finishOneCar(int carid) {
        OnlineCarNum--;
        FinishedCarNum++;
        FinishedCarList.push_back(carid);
        for (int i = 0; i < OnlineCarList.size(); ++i) {
            if (OnlineCarList[i] == carid) {
                OnlineCarList.erase(OnlineCarList.begin() + i);
                break;
            }
        }

    }

    void startOneCar(int carid) {
        OnlineCarNum++;
        OfflineCarNum--;
        OnlineCarList.push_back(carid);
        for (int i = 0; i < OfflineCarList.size(); ++i) {
            if (OfflineCarList[i] == carid) {
                OfflineCarList.erase(OfflineCarList.begin() + i);
                break;
            }
        }

    }

    void updateTime() {
        Time++;
    }

    int getTime() {
        return Time;
    }

    void displayCarList() {
        std::cout << '\n' << "Offline Car List:" << '\n';
        for (int i = 0; i < OfflineCarList.size(); ++i) {
            std::cout << OfflineCarList[i] << '\t';
        }

        std::cout << '\n' << "Online Car List:" << '\n';
        for (int i = 0; i < OnlineCarList.size(); ++i) {
            std::cout << OnlineCarList[i] << '\t';
        }

        std::cout << '\n' << "Finished Car List:" << '\n';
        for (int i = 0; i < FinishedCarList.size(); ++i) {
            std::cout << FinishedCarList[i] << '\t';
        }
    }


};

#endif //CODECRAFT_2019_ELEMENT_H
