#include "iostream"
#include <fstream>
#include <vector>
#include "include/element.h"

std::vector<road> roadVector;
std::vector<cross> crossVector;
std::vector<car> carVector;
std::vector<car> answerVector;
systemInfo System;

int firstRoadID = 9999;
int firstCrossID = 9999;
int firstCarID = 9999;
int firstAnswerID = 9999;


int StringSplit(std::vector<std::string> &dst, const std::string &src, const std::string &separator) {
    if (src.empty() || separator.empty())
        return 0;

    int nCount = 0;
    std::string temp;
    size_t pos = 0, offset = 0;

    // 分割第1~n-1个
    while ((pos = src.find_first_of(separator, offset)) != std::string::npos) {
        temp = src.substr(offset, pos - offset);
        if (temp.length() > 0) {
            dst.push_back(temp);
            nCount++;
        }
        offset = pos + 1;
    }

    // 分割第n个
    temp = src.substr(offset, src.length() - offset);
    if (temp.length() > 0) {
        dst.push_back(temp);
        nCount++;
    }

    return nCount;
}

int getMin(int a, int b) {
    return a < b ? a : b;
}

std::vector<nextMove> getNextPossibleMove(int thisRoadID, int thisRoadOri) {
    std::vector<nextMove> nextmove;

    int thisCrossID = roadVector[thisRoadID].getSimplexRoadEndCrossID(thisRoadOri);
    for (int i = 0; i < crossVector[thisCrossID].NumOfOutRoads; i++) {
        int nextRoadID = crossVector[thisCrossID].sortedOutRoadIDList[i];
        if (nextRoadID != thisRoadID) {
            nextMove tmp;
            tmp.roadID = nextRoadID;
            if (roadVector[nextRoadID].getStartCrossID() == thisCrossID) {
                tmp.crossID = roadVector[nextRoadID].getEndCrossID();
            } else {
                tmp.crossID = roadVector[nextRoadID].getStartCrossID();
            }
            nextmove.push_back(tmp);

        }
    }

    if (nextmove.size() == 0) {
        std::cout << "[INFO] No road to exit." << '\n';
    }

    return nextmove;

};

CarPlan getNextStepPlan(int thisRoadID, int thisRoadOri, nextMove move) {
    CarPlan plan;
    plan.roadID = thisRoadID;
    plan.orientation = thisRoadOri;
    if (roadVector[thisRoadID].getSimplexRoadNextRoadID(thisRoadOri, LEFT) == move.roadID) {
        plan.nextDirection = LEFT;
    } else if (roadVector[thisRoadID].getSimplexRoadNextRoadID(thisRoadOri, RIGHT) == move.roadID) {
        plan.nextDirection = RIGHT;
    } else if (roadVector[thisRoadID].getSimplexRoadNextRoadID(thisRoadOri, STRAIGHT) == move.roadID) {
        plan.nextDirection = STRAIGHT;
    }

    return plan;

}

int getRoadCarNum(int roadid, int ori) {
    int num = 0;
    for (int i = 0; i < roadVector[roadid].getLaneNum(); ++i) {
        for (int j = 0; j < roadVector[roadid].getRoadLength(); ++j) {
            if (roadVector[roadid].getGridState(ori, i, j) != -1) {
                num++;
            }
        }
    }
    return num;
}

void readRoadConfigFile(const std::string &path) {
    ////////////////////////// 读入道路文件 road.txt //////////////////////////////

    std::ifstream configFileRoad(path);
    if (!configFileRoad.is_open()) {
        std::cout << "Fail to open files." << std::endl;
    } else {

        int lineNum = 0;
        std::string temp;
        std::vector<std::string> element;
        std::string separator(", ");

        while (getline(configFileRoad, temp)) {
            if (lineNum == 0) {
                lineNum++;
                continue;
            }

            temp.erase(temp.begin());
            temp.pop_back();

            StringSplit(element, temp, separator);

            int roadID = atoi(element[0].c_str());
            int len = atoi(element[1].c_str());
            int maxSpeed = atoi(element[2].c_str());
            int lane = atoi(element[3].c_str());
            int startpoint = atoi(element[4].c_str());
            int endpoint = atoi(element[5].c_str());
            int isDuplex = atoi(element[6].c_str());

            // 记录首个路口的ID
            if (lineNum == 1) {
                firstRoadID = roadID;
            }

            element.clear();

            lineNum++;

            road tmp(roadID, isDuplex, len, lane, startpoint, endpoint, maxSpeed);
            roadVector.push_back(tmp);
        }

        configFileRoad.close();
    }

    std::cout << "Size of RoadVector: " << roadVector.size() << '\n';
}

void readCrossConfigFile(const std::string &path) {
    std::ifstream configFileCross(path);
    if (!configFileCross.is_open()) {
        std::cout << "Fail to open files." << std::endl;
    } else {

        int lineNum = 0;
        std::string temp;
        std::vector<std::string> element;
        std::string separator(", ");

        while (getline(configFileCross, temp)) {
            if (lineNum == 0) {
                lineNum++;
                continue;
            }

            temp.erase(temp.begin());
            temp.pop_back();

            StringSplit(element, temp, separator);

            int crossID = atoi(element[0].c_str());
            int r1 = atoi(element[1].c_str());
            int r2 = atoi(element[2].c_str());
            int r3 = atoi(element[3].c_str());
            int r4 = atoi(element[4].c_str());

            if (lineNum == 1) {
                firstCrossID = crossID;
            }

            element.clear();

            cross tmp(crossID, r1, r2, r3, r4);
            crossVector.push_back(tmp);

            lineNum++;
        }

        configFileCross.close();
    }

    std::cout << "Size of CrossVector: " << crossVector.size() << '\n';
}

void readCarConfigFile(const std::string &path) {

    std::ifstream configFileCar(path);
    if (!configFileCar.is_open()) {
        std::cout << "Fail to open files." << std::endl;
    } else {

        int lineNum = 0;
        std::string temp;
        std::vector<std::string> element;
        std::string separator(", ");

        while (getline(configFileCar, temp)) {
            if (lineNum == 0) {
                lineNum++;
                continue;
            }

            temp.erase(temp.begin());
            temp.pop_back();

            StringSplit(element, temp, separator);

            int carID = atoi(element[0].c_str());
            int startpoint = atoi(element[1].c_str());
            int endpoint = atoi(element[2].c_str());
            int maxSpeed = atoi(element[3].c_str());
            int planTime = atoi(element[4].c_str());

            if (lineNum == 1) {
                firstCarID = carID;
            }

            element.clear();

            car tmp(carID, maxSpeed, startpoint, endpoint, planTime);
            carVector.push_back(tmp);

            lineNum++;
        }

        configFileCar.close();
    }

    std::cout << "Size of CarVector: " << carVector.size() << '\n';
}

void readAnswerFiles(const std::string &path) {
    std::ifstream configFileAnswer(path);
    if (!configFileAnswer.is_open()) {
        std::cout << "Fail to open files." << std::endl;
    } else {

        int lineNum = 0;
        std::string temp;
        std::vector<std::string> element;
        std::string separator(", ");
        while (getline(configFileAnswer, temp)) {
            if (lineNum == 0) {
                lineNum++;
                continue;
            }

            temp.erase(temp.begin());
            temp.pop_back();

            StringSplit(element, temp, separator);

            if (lineNum == 1) {
                firstAnswerID = atoi(element[0].c_str());
            }

            int carID = -1;
            int i = 0;

            while (i < element.size()) {
                if (i == 0) {
                    carID = atoi(element[i].c_str()) - firstAnswerID;
                } else if (i == 1) {
                    carVector[lineNum - 1].PlannedTime = atoi(element[i].c_str());
                } else {
                    if (carID == carVector[lineNum - 1].ID) {
                        CarPlan tmpcarplan;
                        tmpcarplan.roadID = atoi(element[i].c_str()) - firstRoadID;
                        tmpcarplan.orientation = -1;
                        tmpcarplan.nextDirection = -1;
                        carVector[lineNum - 1].pushCarPlan(tmpcarplan); // push plan.
                    } else {
                        std::cout << "[Error] CarID conflicts." << std::endl;
                        exit(14);
                    }
                }

                ++i;
            }

            lineNum++;

            element.clear();
        }

        configFileAnswer.close();

        if (lineNum != carVector.size() + 1) {
            std::cout << "[ERROR] Unplanned Car" << std::endl;
            exit(13);
        }
    }

    // 更新车辆规划参数
    for (int i = 0; i < carVector.size(); ++i) {
        int sp = carVector[i].StartPoint;
        int planSize = carVector[i].Plan.size();
        if (planSize < 1) {
            std::cout << "[WARNING] CAR " << carVector[i].ID << " doesn't need to be planned?" << '\n';
            continue;
        }

        // 处理初始路口
        int firstRoadid = carVector[i].Plan[0].roadID;
        if (sp == roadVector[firstRoadid].getStartCrossID()) {
            carVector[i].Plan[0].orientation = 0;
        } else if (sp == roadVector[firstRoadid].getEndCrossID()) {
            carVector[i].Plan[0].orientation = 1;
        } else {
            std::cout << "[ERROR] CAR " << carVector[i].ID << " & cross unmatched1." << '\n';
            exit(12);
        }

        // 处理后续路口
        int iter = 1;
        while (iter < planSize) {
            int thisRoadID = carVector[i].Plan[iter].roadID;
            int lastRoadID = carVector[i].Plan[iter - 1].roadID;
            int lastRoadOrientation = carVector[i].Plan[iter - 1].orientation;
            if (thisRoadID == roadVector[lastRoadID].getSimplexRoadNextRoadID(lastRoadOrientation, LEFT)) {
                carVector[i].Plan[iter - 1].nextDirection = LEFT;
            } else if (thisRoadID == roadVector[lastRoadID].getSimplexRoadNextRoadID(lastRoadOrientation, STRAIGHT)) {
                carVector[i].Plan[iter - 1].nextDirection = STRAIGHT;
            } else if (thisRoadID == roadVector[lastRoadID].getSimplexRoadNextRoadID(lastRoadOrientation, RIGHT)) {
                carVector[i].Plan[iter - 1].nextDirection = RIGHT;
            } else {
                std::cout << "[ERROR] CAR " << carVector[i].ID << " & cross unmatched2." << '\t' << lastRoadID << '\n';
                exit(12);
            }

            int thisCrossID = roadVector[lastRoadID].getSimplexRoadEndCrossID(lastRoadOrientation); //相当于sp
            if (thisCrossID == roadVector[thisRoadID].getStartCrossID()) {
                carVector[i].Plan[iter].orientation = 0;
            } else if (thisCrossID == roadVector[thisRoadID].getEndCrossID()) {
                carVector[i].Plan[iter].orientation = 1;
            } else {
                std::cout << "[ERROR] CAR " << carVector[i].ID << " & cross unmatched3." << '\n';
                exit(12);
            }

            iter++;
        }
    }
}

void updateMapIDInfo() {
    /////// 减去首 ID,得到新的以0开始的ID作为索引 ////////////////
    for (int i = 0; i < roadVector.size(); ++i) {
        roadVector[i].updateRoadID(firstRoadID);
        roadVector[i].updateCrossID(firstCrossID);
    }
    for (int i = 0; i < crossVector.size(); ++i) {
        crossVector[i].updateRoadID(firstRoadID);
        crossVector[i].updateCrossID(firstCrossID);
    }
    for (int i = 0; i < carVector.size(); ++i) {
        carVector[i].updateCarID(firstCarID);
        carVector[i].updateCrossID(firstCrossID);
    }

    /////// 更新cross文件中的针对于某一路口的进入该路口的道路id的排序 ////////////////

    for (int i = 0; i < crossVector.size(); ++i) {
        crossVector[i].sortRoadID();
        for (int k = 0; k < crossVector[i].getNumOfRoads(); ++k) {
            int eachRoadID = crossVector[i].sortedRoadIDList[k];
            if (roadVector[eachRoadID].isDuplex()) {
                crossVector[i].sortedInRoadIDList.push_back(eachRoadID);
                crossVector[i].NumOfInRoads++;
                crossVector[i].sortedOutRoadIDList.push_back(eachRoadID);
                crossVector[i].NumOfOutRoads++;
            } else {
                if (roadVector[eachRoadID].getEndCrossID() == crossVector[i].getCrossID()) {
                    crossVector[i].sortedInRoadIDList.push_back(eachRoadID);
                    crossVector[i].NumOfInRoads++;
                } else if (roadVector[eachRoadID].getStartCrossID() == crossVector[i].getCrossID()) {
                    crossVector[i].sortedOutRoadIDList.push_back(eachRoadID);
                    crossVector[i].NumOfOutRoads++;
                }
            }
        }

        ///// Display /////////////////////
        for (int k = 0; k < 4; ++k) {
            std::cout << crossVector[i].getRoadIDSet(k) << '\t';
        }
        std::cout << '\t';
        for (int k = 0; k < crossVector[i].getNumOfRoads(); ++k) {
            int eachRoadID = crossVector[i].sortedRoadIDList[k];
            std::cout << eachRoadID << '\t';
        }
        std::cout << '\t' << '\t' << '\t';
        for (int k = 0; k < crossVector[i].NumOfInRoads; ++k) {
            int eachRoadID = crossVector[i].sortedInRoadIDList[k];
            std::cout << eachRoadID << '\t';
        }
        std::cout << '\n';
        /////////////////////////////////////
    }
}

void buildMap() {
    ////////////////////////// 建立全局地图关系 //////////////////////////////

    DynamicArray roadConnectionInfo(crossVector.size(), crossVector.size());
    for (int i = 0; i < roadVector.size(); ++i) {
        int tmpx = roadVector[i].getStartCrossID();
        int tmpy = roadVector[i].getEndCrossID();
        if (roadVector[i].isDuplex()) {
            roadConnectionInfo.setValue(tmpx, tmpy, roadVector[i].getRoadID());
            roadConnectionInfo.setValue(tmpy, tmpx, roadVector[i].getRoadID());
        } else {
            roadConnectionInfo.setValue(tmpx, tmpy, roadVector[i].getRoadID());
        }
    }

    //    roadConnectionInfo.displayArray();

    // 填写下一路口的各道路的相对方向。
    for (int i = 0; i < roadVector.size(); ++i) { //60
        if (roadVector[i].isDuplex()) {
            // road0
            int tmpCrossID = roadVector[i].getEndCrossID();
            for (int j = 0; j < crossVector.size(); ++j) { // 36
                int tmpNextRoadID = roadConnectionInfo.getValue((tmpCrossID), j);
                //				int tmpNextRoadID;
                if (tmpNextRoadID < 0) { //没有路
                    continue;
                } else {
                    int ptr_self = 999;
                    int ptr_next = 999;
                    for (int k = 0; k < 4; ++k) {
                        int tmpSetID = crossVector[tmpCrossID].getRoadIDSet(k);
                        if (tmpSetID == roadVector[i].getRoadID()) {
                            ptr_self = k;
                        } else if (tmpSetID == tmpNextRoadID) {
                            ptr_next = k;
                        }
                    }

                    if ((ptr_next - ptr_self) == -1 || (ptr_next - ptr_self) == 3) {
                        roadVector[i].setSimplexRoadNextRoadID(0, RIGHT, tmpNextRoadID);
                    } else if ((ptr_next - ptr_self) == -2 || (ptr_next - ptr_self) == 2) {
                        roadVector[i].setSimplexRoadNextRoadID(0, STRAIGHT, tmpNextRoadID);
                    } else if ((ptr_next - ptr_self) == -3 || (ptr_next - ptr_self) == 1) {
                        roadVector[i].setSimplexRoadNextRoadID(0, LEFT, tmpNextRoadID);
                    }
                }
            }

            //road1
            tmpCrossID = roadVector[i].getStartCrossID();
            for (int j = 0; j < crossVector.size(); ++j) { // 36
                int tmpNextRoadID = roadConnectionInfo.getValue((tmpCrossID), j);
                if (tmpNextRoadID < 0) { //没有路
                    continue;
                } else {
                    int ptr_self = 999;
                    int ptr_next = 999;
                    for (int k = 0; k < 4; ++k) {
                        int tmpSetID = crossVector[tmpCrossID].getRoadIDSet(k);
                        if (tmpSetID == roadVector[i].getRoadID()) {
                            ptr_self = k;
                        } else if (tmpSetID == tmpNextRoadID) {
                            ptr_next = k;
                        }
                    }

                    /// 顺时针
                    if ((ptr_next - ptr_self) == -1 || (ptr_next - ptr_self) == 3) {
                        roadVector[i].setSimplexRoadNextRoadID(1, RIGHT, tmpNextRoadID);
                    } else if ((ptr_next - ptr_self) == -2 || (ptr_next - ptr_self) == 2) {
                        roadVector[i].setSimplexRoadNextRoadID(1, STRAIGHT, tmpNextRoadID);
                    } else if ((ptr_next - ptr_self) == -3 || (ptr_next - ptr_self) == 1) {
                        roadVector[i].setSimplexRoadNextRoadID(1, LEFT, tmpNextRoadID);
                    }
                }
            }
        } else {
            int tmpCrossID = roadVector[i].getEndCrossID();
            for (int j = 0; j < crossVector.size(); ++j) { // 36
                int tmpNextRoadID = roadConnectionInfo.getValue((tmpCrossID), j);
                if (tmpNextRoadID < 0) { //没有路
                    continue;
                } else {
                    int ptr_self = 999;
                    int ptr_next = 999;
                    for (int k = 0; k < 4; ++k) {
                        int tmpSetID = crossVector[tmpCrossID].getRoadIDSet(k);
                        if (tmpSetID == roadVector[i].getRoadID()) {
                            ptr_self = k;
                        } else if (tmpSetID == tmpNextRoadID) {
                            ptr_next = k;
                        }
                    }

                    if ((ptr_next - ptr_self) == -1 || (ptr_next - ptr_self) == 3) {
                        roadVector[i].setSimplexRoadNextRoadID(0, RIGHT, tmpNextRoadID);
                    } else if ((ptr_next - ptr_self) == -2 || (ptr_next - ptr_self) == 2) {
                        roadVector[i].setSimplexRoadNextRoadID(0, STRAIGHT, tmpNextRoadID);
                    } else if ((ptr_next - ptr_self) == -3 || (ptr_next - ptr_self) == 1) {
                        roadVector[i].setSimplexRoadNextRoadID(0, LEFT, tmpNextRoadID);
                    }
                }
            }
        }
    }
}

void updateState(systemInfo *&system) {

    system->updateTime();
    std::cout << "/------- SystemTime: " << system->getTime() << " Start -------\\" << '\n';


    if (system->getOnlineCarNum() > 0) {
        /// 第一步：调度单个道路上的车辆 ///////////
        for (int i = 0; i < roadVector.size(); ++i) // 道路id升序
        {
            int thisRoadSpeedLimit = roadVector[i].getRoadMaxSpeed();

            //////// 单向车道 & 双向车道的road0
            for (int j = 0; j < roadVector[i].getRoadLength(); ++j) //第一(0)排到最后一排
            {
                for (int k = 0; k < roadVector[i].getLaneNum(); ++k) //第一(0)车道到最后车道
                {
                    int thisCarID = roadVector[i].getGridState(0, k, j);
                    int thisCarSpeedLimit = carVector[thisCarID].MaxCarSpeed;
                    if (thisCarID != -1) //判断当前格子中有没有车
                    {
                        // 获取车辆的信息：朝向，下一步方向，当前可用车速，车辆位置
                        // int planStep=carVector[thisCarID].PlanStep;
                        // int ori=carVector[thisCarID].Plan[planStep].orientation;
                        // int nextDir=carVector[thisCarID].Plan[planStep].nextDirection;
                        // int speedlimit=carVector[thisCarID].Plan[planStep].thisRoadSpeedLimit;
                        int speedlimit =
                                thisCarSpeedLimit < thisRoadSpeedLimit ? thisCarSpeedLimit : thisRoadSpeedLimit;

                        //判断能否出路口？
                        bool exitRoadFlag = false;
                        if (j - speedlimit < 0) {
                            exitRoadFlag = true;
                        }

                        // 判断前方是否有阻挡？
                        bool blockFlag = false;
                        int distToFrontCar = -1; // 与前车距离
                        int stopCarID = -1;
                        for (int p = j - 1; p >= 0; --p) //从后往前寻找本车道最近的前车位置
                        {
                            if (roadVector[i].getGridState(0, k, p) != -1) {
                                distToFrontCar = j - p;
                                if (distToFrontCar <= speedlimit) //被阻挡！！！！！！！！！！！！！Attention
                                {
                                    stopCarID = roadVector[i].getGridState(0, k, j - distToFrontCar);
                                    blockFlag = true;
                                }
                                break;
                            }
                        }


                        if (!blockFlag) //没有阻挡
                        {

                            if (exitRoadFlag) //可以出路口
                            {
                                //标记为等待车辆
                                carVector[thisCarID].CarState = 1;
                            } else //不能出路口
                            {
                                //标记为终止状态、确定终止位置
                                carVector[thisCarID].CarState = 0;
                                // 更新车辆列表中的信息
                                carVector[thisCarID].presentPosition.laneID = k;
                                carVector[thisCarID].presentPosition.orientation = 0;
                                carVector[thisCarID].presentPosition.positionID = j - speedlimit;
                                carVector[thisCarID].presentPosition.roadID = roadVector[i].getRoadID();
                                // 更新道路地图列表中的信息
                                roadVector[i].changeGridState(0, k, j, -1);
                                roadVector[i].changeGridState(0, k, j - speedlimit, thisCarID);
                            }
                        } else //有阻挡
                        {
                            if (carVector[stopCarID].CarState == 1) //阻挡的车是等待车辆
                            {
                                //标记为等待车辆
                                carVector[thisCarID].CarState = 1;
                            } else //阻挡的车是终止状态车辆
                            {
                                //标记为终止状态、确定终止位置
                                carVector[thisCarID].CarState = 0;
                                // 更新车辆列表中的信息
                                carVector[thisCarID].presentPosition.laneID = k;
                                carVector[thisCarID].presentPosition.orientation = 0;
                                carVector[thisCarID].presentPosition.positionID =
                                        j - distToFrontCar + 1; // Attention!!!!!!!!!!!!!!!!!!!!!
                                carVector[thisCarID].presentPosition.roadID = roadVector[i].getRoadID();
                                // 更新道路地图列表中的信息
                                roadVector[i].changeGridState(0, k, j, -1);
                                roadVector[i].changeGridState(0, k, j - distToFrontCar + 1, thisCarID);
                            }
                        }
                    }
                } // lane1 至 lane n
            }     // 第一排到最后一排

            // 判断是否是双向车道；
            if (roadVector[i].isDuplex()) {
                ////////// road1
                for (int j = 0; j < roadVector[i].getRoadLength(); ++j) //第一(0)排到最后一排
                {
                    for (int k = 0; k < roadVector[i].getLaneNum(); ++k) //第一(0)车道到最后车道
                    {
                        int thisCarID = roadVector[i].getGridState(1, k, j);
                        int thisCarSpeedLimit = carVector[thisCarID].MaxCarSpeed;
                        if (thisCarID != -1) //判断当前格子中有没有车
                        {
                            // 获取车辆的信息：朝向，下一步方向，当前可用车速，车辆位置
                            // int planStep=carVector[thisCarID].PlanStep;
                            // int ori=carVector[thisCarID].Plan[planStep].orientation;
                            // int nextDir=carVector[thisCarID].Plan[planStep].nextDirection;
                            // int speedlimit=carVector[thisCarID].Plan[planStep].thisRoadSpeedLimit;
                            int speedlimit =
                                    thisCarSpeedLimit < thisRoadSpeedLimit ? thisCarSpeedLimit : thisRoadSpeedLimit;

                            //判断能否出路口？
                            bool exitRoadFlag = false;
                            if (j - speedlimit < 0) {
                                exitRoadFlag = true;
                            }


                            // 判断前方是否有阻挡？
                            // 从本车道的第一排开始往自己遍历，找到最近的车
                            bool blockFlag = false;
                            int distToFrontCar = 1;
                            int stopCarID = -1;
                            for (int p = j - 1; p >= 0; --p) //从后往前寻找本车道最近的前车位置
                            {
                                if (roadVector[i].getGridState(1, k, p) != -1) {
                                    distToFrontCar = j - p;
                                    if (distToFrontCar <= speedlimit) //被阻挡！！！！！！！！！！！！！
                                    {
                                        stopCarID = roadVector[i].getGridState(1, k, j - distToFrontCar);
                                        blockFlag = true;
                                    }
                                    break;
                                }
                            }


                            if (!blockFlag) //没有阻挡
                            {
                                if (exitRoadFlag) //可以出路口
                                {
                                    //标记为等待车辆
                                    carVector[thisCarID].CarState = 1;
                                } else //不能出路口
                                {
                                    //标记为终止状态、确定终止位置
                                    carVector[thisCarID].CarState = 0;
                                    // 更新车辆列表中的信息
                                    carVector[thisCarID].presentPosition.laneID = k;
                                    carVector[thisCarID].presentPosition.orientation = 1;
                                    carVector[thisCarID].presentPosition.positionID = j - speedlimit;
                                    carVector[thisCarID].presentPosition.roadID = roadVector[i].getRoadID();
                                    // 更新道路地图列表中的信息
                                    roadVector[i].changeGridState(1, k, j, -1);
                                    roadVector[i].changeGridState(1, k, j - speedlimit, thisCarID);
                                }
                            } else //有阻挡
                            {
                                if (carVector[stopCarID].CarState == 1) //阻挡的车是等待车辆
                                {
                                    //标记为等待车辆
                                    carVector[thisCarID].CarState = 1;
                                } else //阻挡的车是终止状态车辆
                                {
                                    //标记为终止状态、确定终止位置
                                    carVector[thisCarID].CarState = 0;
                                    // 更新车辆列表中的信息
                                    carVector[thisCarID].presentPosition.laneID = k;
                                    carVector[thisCarID].presentPosition.orientation = 1;
                                    carVector[thisCarID].presentPosition.positionID = j - distToFrontCar + 1;
                                    carVector[thisCarID].presentPosition.roadID = roadVector[i].getRoadID();
                                    // 更新道路地图列表中的信息
                                    roadVector[i].changeGridState(1, k, j, -1);
                                    roadVector[i].changeGridState(1, k, j - distToFrontCar + 1, thisCarID);
                                }
                            }
                        }
                    } // lane1 至 lane n
                }     // 第一排到最后一排
            }

        } //第一步

        /// 第二步：处理路口、道路中处于等待状态的车辆。
        system->carNumInWaiting = 0;
        for (int i = 0; i < carVector.size(); ++i) {
            if (carVector[i].CarState == 1) {
                system->carNumInWaiting++;
            }
        }

        while (system->carNumInWaiting > 0) //还有车辆没动过,处于等待状态
        {

            for (int i = 0; i < crossVector.size(); ++i) // 以crossID升序循环每一路口
            {
                int lastWaitingNum_cross = crossVector[i].carNumInWaiting;
                crossVector[i].carNumInWaiting = 0;
                for (int iter = 0; iter < carVector.size(); ++iter) {
                    if (carVector[iter].CarState == 1) {
                        int tmpRoadid = carVector[iter].presentPosition.roadID;
                        int tmpRoadori = carVector[iter].presentPosition.orientation;
                        if (roadVector[tmpRoadid].getSimplexRoadEndCrossID(tmpRoadori) ==
                            crossVector[i].getCrossID()) {
                            crossVector[i].carNumInWaiting++;
                        }
                    }
                }

                while (crossVector[i].carNumInWaiting > 0) {

                    for (int j = 0; j < crossVector[i].NumOfInRoads; ++j) //以升序循环本路口的道路
                    {
                        int thisRoadID = crossVector[i].sortedInRoadIDList[j];
                        // 判断本道路的相对路口id的方向
                        int thisRoadOri = 0;
                        if (roadVector[thisRoadID].getStartCrossID() == crossVector[i].getCrossID()) {
                            thisRoadOri = 1;
                        }
                        int thisRoadSpeedLimit = roadVector[thisRoadID].getRoadMaxSpeed();


                        // 本road的优先级顺序
                        bool thisRoadBreakFlag = false;
                        for (int k = 0; k < roadVector[thisRoadID].getRoadLength(); ++k) //按排循环
                        {
                            for (int m = 0; m < roadVector[thisRoadID].getLaneNum(); ++m) //按车道顺序循环
                            {
                                int thisGridID = roadVector[thisRoadID].getGridState(thisRoadOri, m, k);
                                if (thisGridID != -1 && carVector[thisGridID].CarState == 1) // 锁定当前需要调度的车
                                {

                                    int thisCarSpeedLimit = carVector[thisGridID].MaxCarSpeed;
                                    int speedLimit = thisCarSpeedLimit < thisRoadSpeedLimit ? thisCarSpeedLimit
                                                                                            : thisRoadSpeedLimit;

                                    // 判断本road的前进方向上是否有阻挡
                                    bool blockFlag = false;
                                    int distToFrontCar = 1; // 与前车距离
                                    int stopCarID = -1;
                                    for (int q = k - 1; q >= 0; --q) {
                                        if (roadVector[thisRoadID].getGridState(thisRoadOri, m, q) != -1) {
                                            distToFrontCar = k - q;
                                            if (distToFrontCar <= speedLimit) //被阻挡！！！！！！！！！！！！！！Attention
                                            {
                                                stopCarID = roadVector[thisRoadID].getGridState(thisRoadOri, m,
                                                                                                k - distToFrontCar);
                                                blockFlag = true;
                                            }
                                            break;
                                        }
                                    }


                                    if (blockFlag) //有阻挡，无论如何都无法出路口，不必讨论冲突问题。可转化为第一步的(d)
                                    {

                                        //标记为终止状态、确定终止位置
                                        carVector[thisGridID].CarState = 0;
                                        // 更新车辆列表中的信息
                                        carVector[thisGridID].presentPosition.laneID = m;
                                        carVector[thisGridID].presentPosition.orientation = thisRoadOri;
                                        carVector[thisGridID].presentPosition.positionID =
                                                k - distToFrontCar + 1; // Attention!!!!!!!!!!!!!!!!!!!!!
                                        carVector[thisGridID].presentPosition.roadID = thisRoadID;
                                        // 更新道路地图列表中的信息
                                        roadVector[thisRoadID].changeGridState(thisRoadOri, m, k, -1);
                                        roadVector[thisRoadID].changeGridState(thisRoadOri, m,
                                                                               k - distToFrontCar + 1, thisGridID);
                                    } else //无阻挡
                                    {

                                        // 判断是否具有冲出路口的潜力？
                                        if (speedLimit <= k) // 自身速度有限，无法冲出路口。可转化为第一步的(b)
                                        {
                                            //标记为终止状态、确定终止位置
                                            carVector[thisGridID].CarState = 0;
                                            // 更新车辆列表中的信息
                                            carVector[thisGridID].presentPosition.laneID = m;
                                            carVector[thisGridID].presentPosition.orientation = thisRoadOri;
                                            carVector[thisGridID].presentPosition.positionID = k - speedLimit;
                                            carVector[thisGridID].presentPosition.roadID = thisRoadID;
                                            // 更新道路地图列表中的信息
                                            roadVector[thisRoadID].changeGridState(thisRoadOri, m, k, -1);
                                            roadVector[thisRoadID].changeGridState(thisRoadOri, m, k - speedLimit,
                                                                                   thisGridID);
                                        } else //有希望冲出路口
                                        {
                                            // 寻找下一条要去的路：roadID、朝向
                                            int thisPlanStep = carVector[thisGridID].PlanStep;
                                            int nextPlanStep = thisPlanStep + 1;

                                            if (nextPlanStep == carVector[thisGridID].Plan.size()) //没有剩余的规划，已到达终点
                                            {
                                                if (roadVector[carVector[thisGridID].presentPosition.roadID].getSimplexRoadEndCrossID(
                                                        carVector[thisGridID].presentPosition.orientation) ==
                                                    carVector[thisGridID].EndPoint) {
                                                    ////////// 到达终点的一系列操作 //////////////////////
                                                    //标记为终止状态、确定终止位置
                                                    carVector[thisGridID].CarState = 0;
                                                    // 更新车辆列表中的信息
                                                    carVector[thisGridID].presentPosition.laneID = -2;
                                                    carVector[thisGridID].presentPosition.orientation = -2;
                                                    carVector[thisGridID].presentPosition.positionID = -2;
                                                    carVector[thisGridID].presentPosition.roadID = -2;
                                                    carVector[thisGridID].FinishedTime = system->getTime();
                                                    carVector[thisGridID].PlanStep = -2;
                                                    // 更新道路地图列表中的信息
                                                    roadVector[thisRoadID].changeGridState(thisRoadOri, m, k, -1);
                                                    // 系统信息
                                                    system->finishOneCar(thisGridID);
                                                    std::cout << "[CAR INFO] Car " << thisGridID + 1
                                                              << " has reached the end." << '\n';
                                                } else {
                                                    std::cout << "[ERROR] Car " << thisGridID + 1
                                                              << " has no next plan input." << '\n';
                                                    exit(9);
                                                }
                                            } else {

                                                int nextRoadID = carVector[thisGridID].Plan[nextPlanStep].roadID;
                                                if (nextRoadID == carVector[thisGridID].Plan[thisPlanStep].roadID) {
                                                    std::cout << "[ERROR] Car " << thisGridID + 1
                                                              << " has repeated plan input." << '\n';
                                                    exit(8);
                                                }
                                                int nextRoadOri = 0;
                                                if (roadVector[nextRoadID].getEndCrossID() ==
                                                    crossVector[i].getCrossID()) {
                                                    nextRoadOri = 1;
                                                }

                                                int nextRoadSpeedLimit =
                                                        thisCarSpeedLimit < roadVector[nextRoadID].getRoadMaxSpeed()
                                                        ? thisCarSpeedLimit
                                                        : roadVector[nextRoadID].getRoadMaxSpeed();
                                                // speedLimit: 本条路上的综合限速；nextRoadSpeedLimit: 下条路上的综合限速
                                                int restDist = k;
                                                //在下条路能走的距离
                                                int nextRoadExpectedDist =
                                                        (getMin(speedLimit, nextRoadSpeedLimit) - restDist) > 0 ? (
                                                                getMin(speedLimit, nextRoadSpeedLimit) - restDist)
                                                                                                                : 0;

                                                if (nextRoadExpectedDist == 0) //由于下条路的限速，出路口失败，停在了本道路的路口边缘。
                                                {
                                                    //标记为终止状态、确定终止位置
                                                    carVector[thisGridID].CarState = 0;
                                                    // 更新车辆列表中的信息
                                                    carVector[thisGridID].presentPosition.laneID = m;
                                                    carVector[thisGridID].presentPosition.orientation = thisRoadOri;
                                                    carVector[thisGridID].presentPosition.positionID = 0;
                                                    carVector[thisGridID].presentPosition.roadID = thisRoadID;
                                                    // 更新道路地图列表中的信息
                                                    roadVector[thisRoadID].changeGridState(thisRoadOri, m, k, -1);
                                                    roadVector[thisRoadID].changeGridState(thisRoadOri, m, 0,
                                                                                           thisGridID);
                                                } else // 不会因为下条路的限速而无法冲出路口
                                                {
                                                    //////////////// 准备通过路口，开始判断冲突。//////////////////
                                                    bool conflictFlag = false;

                                                    if (carVector[thisGridID].Plan[thisPlanStep].nextDirection ==
                                                        STRAIGHT) {
                                                        conflictFlag = false;
                                                    } else if (
                                                            carVector[thisGridID].Plan[thisPlanStep].nextDirection ==
                                                            LEFT) {
                                                        int rightRoadID = roadVector[thisRoadID].getSimplexRoadNextRoadID(
                                                                thisRoadOri, RIGHT);
                                                        int rightRoadOri = 0;
                                                        if (roadVector[rightRoadID].getStartCrossID() ==
                                                            crossVector[i].getCrossID()) {
                                                            rightRoadOri = 1;
                                                        }
                                                        // 检查右边道路上的第一优先级的车是否要直行。
                                                        // 右边道路的优先级搜索
                                                        bool rBreakFlag = false;
                                                        for (int rk = 0; rk <
                                                                         roadVector[rightRoadID].getRoadLength();
                                                             ++rk) //按排循环
                                                        {
                                                            for (int rm = 0; rm <
                                                                             roadVector[rightRoadID].getLaneNum();
                                                                 ++rm) //按车道顺序循环
                                                            {
                                                                int rightGridID = roadVector[rightRoadID].getGridState(
                                                                        rightRoadOri, rm, rk);
                                                                if (rightGridID != -1 &&
                                                                    carVector[rightGridID].CarState == 1) {
                                                                    int rightCarThisPlanStep = carVector[rightGridID].PlanStep;
                                                                    int rightCarNextPlanStep =
                                                                            rightCarThisPlanStep + 1;
                                                                    if (carVector[rightGridID].Plan[rightCarNextPlanStep].nextDirection ==
                                                                        STRAIGHT) // !!!!!!!!!!
                                                                    {
                                                                        conflictFlag = true;
                                                                        rBreakFlag = true;
                                                                        break;
                                                                    } else {
                                                                        conflictFlag = false;
                                                                        rBreakFlag = true;
                                                                        break;
                                                                    }
                                                                }
                                                            }

                                                            if (rBreakFlag) {
                                                                break;
                                                            }
                                                        }
                                                    } else if (
                                                            carVector[thisGridID].Plan[thisPlanStep].nextDirection ==
                                                            RIGHT) {
                                                        // 检查直行冲突：在本车道的左边车道
                                                        int leftRoadID = roadVector[thisRoadID].getSimplexRoadNextRoadID(
                                                                thisRoadOri, LEFT);
                                                        int leftRoadOri = 0;
                                                        if (roadVector[leftRoadID].getStartCrossID() ==
                                                            crossVector[i].getCrossID()) {
                                                            leftRoadOri = 1;
                                                        }
                                                        // 检查左边道路上的第一优先级的车是否要直行。
                                                        // 左边道路的优先级搜索
                                                        bool lBreakFlag = false;
                                                        for (int lk = 0; lk <
                                                                         roadVector[leftRoadID].getRoadLength();
                                                             ++lk) //按排循环
                                                        {
                                                            for (int lm = 0; lm <
                                                                             roadVector[leftRoadID].getLaneNum();
                                                                 ++lm) //按车道顺序循环
                                                            {
                                                                int leftGridID = roadVector[leftRoadID].getGridState(
                                                                        leftRoadOri, lm, lk);
                                                                if (leftGridID != -1 &&
                                                                    carVector[leftGridID].CarState == 1) {
                                                                    int leftCarThisPlanStep = carVector[leftGridID].PlanStep;
                                                                    int leftCarNextPlanStep =
                                                                            leftCarThisPlanStep + 1;

                                                                    conflictFlag =
                                                                            (carVector[leftGridID].Plan[leftCarNextPlanStep].nextDirection ==
                                                                             STRAIGHT);
                                                                    lBreakFlag = true;
                                                                    break;
                                                                }
                                                            }

                                                            if (lBreakFlag) {
                                                                break;
                                                            }
                                                        }

                                                        if (!conflictFlag) //直行无冲突，再检查左转冲突。
                                                        {
                                                            // 检查左转冲突：在本车道的对面车道
                                                            int frontRoadID = roadVector[thisRoadID].getSimplexRoadNextRoadID(
                                                                    thisRoadOri, STRAIGHT);
                                                            int frontRoadOri = 0;
                                                            if (roadVector[frontRoadID].getStartCrossID() ==
                                                                crossVector[i].getCrossID()) {
                                                                frontRoadOri = 1;
                                                            }

                                                            // 检查对面道路上的第一优先级的车是否要左转。
                                                            // 对面道路的优先级搜索
                                                            bool fBreakFlag = false;
                                                            for (int fk = 0; fk <
                                                                             roadVector[frontRoadID].getRoadLength();
                                                                 ++fk) //按排循环
                                                            {
                                                                for (int fm = 0; fm <
                                                                                 roadVector[frontRoadID].getLaneNum();
                                                                     ++fm) //按车道顺序循环
                                                                {
                                                                    int frontGridID = roadVector[frontRoadID].getGridState(
                                                                            frontRoadOri, fm, fk);
                                                                    if (frontGridID != -1 &&
                                                                        carVector[frontGridID].CarState == 1) {
                                                                        int frontCarThisPlanStep = carVector[frontGridID].PlanStep;
                                                                        int frontCarNextPlanStep =
                                                                                frontCarThisPlanStep + 1;

                                                                        conflictFlag =
                                                                                (carVector[frontGridID].Plan[frontCarNextPlanStep].nextDirection ==
                                                                                 LEFT);
                                                                        fBreakFlag = true;
                                                                        break;
                                                                    }
                                                                }

                                                                if (fBreakFlag) {
                                                                    break;
                                                                }
                                                            }
                                                        }
                                                    }
                                                    ////////////// 冲突检查 /////////////////

                                                    if (conflictFlag) //如果有冲突
                                                    {
                                                        // 跳出该road的循环。
                                                        thisRoadBreakFlag = true;
                                                        break;
                                                    } else //如果无冲突
                                                    {
                                                        // 检查预计路线上是否有车辆阻挡，如果有，判断该阻挡会不会让本车无法移动
                                                        bool canMoveFlag = false;
                                                        int possibleX = -1;
                                                        int possibleY = -1;
                                                        for (int x = 0;
                                                             x < roadVector[nextRoadID].getLaneNum(); ++x) {
                                                            for (int y = roadVector[nextRoadID].getRoadLength() - 1;
                                                                 y >= roadVector[nextRoadID].getRoadLength() -
                                                                      nextRoadExpectedDist;
                                                                 --y) {
                                                                if (roadVector[nextRoadID].getGridState(nextRoadOri,
                                                                                                        x,
                                                                                                        y) == -1) {
                                                                    // 更新可能位置
                                                                    possibleX = x;
                                                                    possibleY = y;
                                                                    canMoveFlag = true;
                                                                } else {
                                                                    break;
                                                                }
                                                            }
                                                            if (canMoveFlag) //是否已经找到可能位置
                                                            {
                                                                break;
                                                            }
                                                        }

                                                        if (canMoveFlag) {
                                                            //标记为终止状态、确定终止位置
                                                            carVector[thisGridID].CarState = 0;
                                                            // 更新车辆列表中的信息
                                                            carVector[thisGridID].presentPosition.laneID = possibleX;
                                                            carVector[thisGridID].presentPosition.orientation = nextRoadOri;
                                                            carVector[thisGridID].presentPosition.positionID = possibleY;
                                                            carVector[thisGridID].presentPosition.roadID = nextRoadID;
                                                            // 更新道路地图列表中的信息
                                                            roadVector[thisRoadID].changeGridState(thisRoadOri, m,
                                                                                                   k,
                                                                                                   -1);
                                                            roadVector[nextRoadID].changeGridState(nextRoadOri,
                                                                                                   possibleX,
                                                                                                   possibleY,
                                                                                                   thisGridID);
                                                            // 更新plan
                                                            carVector[thisGridID].PlanStep++;
                                                        } else // 阻挡让本车无法移动,保持等待状态
                                                        {
                                                            // 跳出road循环。
                                                            thisRoadBreakFlag = true;
                                                            break;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            if (thisRoadBreakFlag) {
                                break;
                            }

                        } //本道路
                    }     //本路口

                    // 检查路口是否暂时死锁
                    crossVector[i].carNumInWaiting = 0;
                    for (int iter = 0; iter < carVector.size(); ++iter) {
                        if (carVector[iter].CarState == 1) {
                            int tmpRoadid = carVector[iter].presentPosition.roadID;
                            int tmpRoadori = carVector[iter].presentPosition.orientation;
                            if (roadVector[tmpRoadid].getSimplexRoadEndCrossID(tmpRoadori) ==
                                crossVector[i].getCrossID()) {
                                crossVector[i].carNumInWaiting++;
                            }
                        }
                    }

                    if (crossVector[i].carNumInWaiting == 0) //循环一个路口的road周期,没有等待状态的车辆了。
                    {
                        break;
                    }
                    if (lastWaitingNum_cross == crossVector[i].carNumInWaiting) //循环一个路口的road周期没有任何状态改变
                    {
                        std::cout << "[INFO] Cross " << crossVector[i].getCrossID() << " unchanged with "
                                  << lastWaitingNum_cross << " waiting cars." << std::endl;
                        break;
                    }

                }
            }

            // 检查系统是否死锁
            int lastWaitingNum_system = system->carNumInWaiting;
            system->carNumInWaiting = 0;
            for (int i = 0; i < carVector.size(); ++i) {
                if (carVector[i].CarState == 1) {
                    system->carNumInWaiting++;
                }
            }
            if (lastWaitingNum_system == system->carNumInWaiting && lastWaitingNum_system != 0) //循环一个cross周期没有任何状态改变
            {
                std::cout << "[ERROR] System Locked." << std::endl;
                break;
            }

        } //第二步

    } // 本次在线的车

    if (system->getOfflineCarNum() > 0) {
        for (int i = 0; i < carVector.size(); ++i) {
            if (carVector[i].CarState == -1) //还未出发
            {
                if (carVector[i].PlannedTime <= system->getTime()) //已经达到可出发时间
                {
                    // 判断出发道路上是否有阻挡
                    bool canStartFlag = false;
                    int possibleX = -1;
                    int possibleY = -1;
                    if (carVector[i].Plan.size() > 0) {
                        int nextRoadID = carVector[i].Plan[0].roadID;
                        int nextRoadOri = carVector[i].Plan[0].orientation;
                        int nextRoadExpectedDist = carVector[i].MaxCarSpeed < roadVector[nextRoadID].getRoadMaxSpeed()
                                                   ? carVector[i].MaxCarSpeed
                                                   : roadVector[nextRoadID].getRoadMaxSpeed();
                        for (int x = 0; x < roadVector[nextRoadID].getLaneNum(); ++x) {
                            for (int y = roadVector[nextRoadID].getRoadLength() - 1;
                                 y >= roadVector[nextRoadID].getRoadLength() -
                                      nextRoadExpectedDist;
                                 --y) {
                                if (roadVector[nextRoadID].getGridState(nextRoadOri, x,
                                                                        y) == -1) {
                                    // 更新可能位置
                                    possibleX = x;
                                    possibleY = y;
                                    canStartFlag = true;
                                } else {
                                    break;
                                }
                            }
                            if (canStartFlag) //是否已经找到可能位置
                            {
                                break;
                            }
                        }

                        if (canStartFlag) {
                            //标记为终止状态、确定终止位置
                            carVector[i].CarState = 0;
                            // 更新车辆列表中的信息
                            carVector[i].presentPosition.laneID = possibleX;
                            carVector[i].presentPosition.orientation = nextRoadOri;
                            carVector[i].presentPosition.positionID = possibleY;
                            carVector[i].presentPosition.roadID = nextRoadID;
                            // 更新道路地图列表中的信息
                            roadVector[nextRoadID].changeGridState(nextRoadOri,
                                                                   possibleX, possibleY,
                                                                   i);
                            // 更新plan
                            carVector[i].PlanStep = 0;
                            // 更新实际出发时间
                            carVector[i].ActualTime = system->getTime();
                            //更新系统信息
                            system->startOneCar(i);
                            std::cout << "[CAR INFO] Car " << i << " starts." << '\n';
                        } else // 阻挡让本车无法移动,保持等待状态
                        {
                            // 跳出car循环。
                            break;
                        }
                    } else {
                        std::cout << "[ERROR] Car " << carVector[i].ID << " has no plan input." << '\n';
                        exit(9);
                    }
                }
            }
        }
    } // 本次还未出发的车

    std::cout << "\\_______ SystemTime: " << system->getTime() << " End _________/" << '\n' << '\n' << '\n';


}

void initializeSystem() {
    std::vector<int> carIDList;
    carIDList.reserve(carVector.size());
    for (int i = 0; i < carVector.size(); ++i) {
        carIDList.push_back(carVector[i].ID);
    }

    std::cout << System.getTime() << '\n';
    System.inputCarVector(carIDList);

    System.displayCarList();

    roadVector[0].display();
}

//int main(int argc, char *argv[])
int main() {
    std::cout << "Begin" << std::endl;

    //	if(argc < 5){
    //		std::cout << "please input args: carPath, roadPath, crossPath, answerPath" << std::endl;
    //		exit(1);
    //	}
    //
    //	std::string carPath(argv[1]);
    //	std::string roadPath(argv[2]);
    //	std::string crossPath(argv[3]);
    //	std::string answerPath(argv[4]);
    //
    //	std::cout << "carPath is " << carPath << std::endl;
    //	std::cout << "roadPath is " << roadPath << std::endl;
    //	std::cout << "crossPath is " << crossPath << std::endl;
    //	std::cout << "answerPath is " << answerPath << std::endl;

    std::string roadPath("../config/road.txt");
    std::string crossPath("../config/cross.txt");
    std::string carPath("../config/car.txt");
    std::string answerPath("../config/answer.txt");


    ////////////////////////// 读入道路文件 road.txt /////////////////////////////
    readRoadConfigFile(roadPath);

    ////////////////////////// 读入路口文件 cross.txt ////////////////////////////
    readCrossConfigFile(crossPath);

    ////////////////////////// 读入车辆文件 car.txt //////////////////////////////
    readCarConfigFile(carPath);

    ////////////////////////// 更新map中所有的ID信息 //////////////////////////////
    updateMapIDInfo();

    ////////////////////////// 建立全局地图拓扑关系 ////////////////////////////////
    buildMap();

//    ////////////////// test //////////////////
//    int test_para = 17;
//    std::cout << '\n';
//    std::cout << "Road " << roadVector[test_para].getRoadID() + 5000 << " Direction + " << '\n';
//    std::cout << '\t' << " left:" << '\t' << roadVector[test_para].getSimplexRoadNextRoadID(0, LEFT) << '\n'
//              << '\t' << " straight:" << '\t' << roadVector[test_para].getSimplexRoadNextRoadID(0, STRAIGHT) << '\n'
//              << '\t' << " right:" << '\t' << roadVector[test_para].getSimplexRoadNextRoadID(0, RIGHT) << '\n';
//    std::cout << "Road " << roadVector[test_para].getRoadID() + 5000 << " Direction - " << '\n';
//    std::cout << '\t' << " left:" << '\t' << roadVector[test_para].getSimplexRoadNextRoadID(1, LEFT) << '\n'
//              << '\t' << " straight:" << '\t' << roadVector[test_para].getSimplexRoadNextRoadID(1, STRAIGHT) << '\n'
//              << '\t' << " right:" << '\t' << roadVector[test_para].getSimplexRoadNextRoadID(1, RIGHT) << '\n';
//    ////////////////// test //////////////////


    ////////////////////////// 读入规划文件 answer.txt 并更新车辆规划参数 ///////////////////////////
    // readAnswerFiles(answerPath);

    ////    carVector[4].displayPlan();

    ////////////////////////////////////////////////////////////////////////////////////////////


    initializeSystem();

    systemInfo *p = &System;


    // TODO: 所有即将出发车辆的初始规划 ////////////////////////////////////////////////////////////

//    ///////// test ///////////////
//    CarPlan firstPlan;
//    firstPlan.roadID = 0;
//    firstPlan.orientation = 0;
//    firstPlan.nextDirection = 0;
//
//    carVector[0].pushCarPlan(firstPlan);
//    carVector[1].pushCarPlan(firstPlan);
//    carVector[2].pushCarPlan(firstPlan);
//    carVector[3].pushCarPlan(firstPlan);
//    carVector[4].pushCarPlan(firstPlan);
//    carVector[5].pushCarPlan(firstPlan);
//    carVector[6].pushCarPlan(firstPlan);
//    carVector[7].pushCarPlan(firstPlan);
//    ///////// test ///////////////


    do {
        //////// 状态更新 //////////////////////////////////////////////////////////////////////
        updateState(p);

        // TODO: ///////// 获取当前地图信息 //////////////////////////////////////////////////////
        // 获取某一路上的车辆数目： getRoadCarNum(int roadid, int ori)

        // 获取某一车所在路上的车辆数目：
        // roadID = carVector[carID].getPresnetRoadID();
        // roadOri = carVector[carID].getPresnetRoadOri();
        // 再使用函数 getRoadCarNum(roadID, roadOri)；


        // TODO: /////////// 核心算法 ///////////////////////////////////////////////////////////


        // TODO: //////// 写入所有车的下一步规划 ///////////////////////////////////////////////////
        // 使用函数 getNextPossibleMove() 得到车辆走出这一路口时可能的走向。
        // 输入为本roadID、以及朝向(0或1)，结果返回在 std::vector<nextMove> 里。
        // 其中，nextMove结构体中，nextMove.crossID 代表下一路口的ID，nextMove.roadID 代表下一道路的ID。

        // 当前车辆可在 std::vector<nextMove> 中，按照算法需要，任意选择一种去向，假设为 move_A (nextMove类型)。
        // 选择完某种去向后，利用函数 getNextStepPlan() 把 move_A 转化为 plan_A (CarPlan 类型)
        // 例如：plan_A = getNextStepPlan(路id，路朝向，move_A)

        // 将生成的单个 plan_A 压入到某辆车的 PlanList 中
        // 例如：carVector[carID].pushCarPlan(plan_A)

        // 依此类推，为所有在线的车辆push一个规划。

//        ////////// test /////////////////////////////////
//        nextMove moveX;
//        moveX.roadID = 1;
//        moveX.crossID = 2;
//
//        CarPlan planX;
//        planX = getNextStepPlan(1,0,moveX);
//
//        carVector[0].pushCarPlan(planX);
//        carVector[1].pushCarPlan(planX);
//        carVector[2].pushCarPlan(planX);
//        carVector[3].pushCarPlan(planX);
//        carVector[4].pushCarPlan(planX);
//        carVector[5].pushCarPlan(planX);
//        carVector[6].pushCarPlan(planX);
//        carVector[7].pushCarPlan(planX);
//        ////////// test /////////////////////////////////



//    } while (System.getOnlineCarNum() > 0); // 运行完所有车辆
    } while (System.getTime() < 2); // 时间单步

    /////////////////////////////////////////////////////////////////////////////////////////////////

    roadVector[1].display();


    std::cout << "Finished within " << System.getTime() << " seconds";

    // TODO:read input filebuf
    // TODO:processs
    // TODO:write output file

    return 0;
}
