package utils;
import pojo.Car;
import pojo.Cross;
import pojo.Road;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;

public class ReadData {
    static public List<Road> readRoad(String path) {
        List<Road> list = new ArrayList<>();
        String line;
        FileReader fileReader = FileUtil.getFileReader(path);
        BufferedReader bufferedReader = FileUtil.getBufferedReader(fileReader);
        try {
            while ((line=bufferedReader.readLine())!=null) {
                //System.out.println(line);
                String[] info = getInfo(line);
                //判断info的信息是否与Road的信息一样多，如果是则填充Road对象并加到list里
                if (Road.numberPara==info.length) {
                    Road road = new Road();
                    road.setId(Integer.parseInt(info[0]));
                    road.setLength(Integer.parseInt(info[1]));
                    road.setSpeedLimit(Integer.parseInt(info[2]));
                    road.setLaneNums(Integer.parseInt(info[3]));
                    road.setBeginId(Integer.parseInt(info[4]));
                    road.setEndId(Integer.parseInt(info[5]));
                    if (info[6].equals("1")) {
                        road.setBidirectional(true);
                    } else {
                        road.setBidirectional(false);
                    }
                    list.add(road);
                }
            }
            return list;
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            //关闭资源
            FileUtil.closeBufferedReader(bufferedReader);
        }
        return list;
    }

    static public List<Car> readCar (String path) {
        List<Car> list = new ArrayList<>();
        String line;
        FileReader fileReader = FileUtil.getFileReader(path);
        BufferedReader bufferedReader = FileUtil.getBufferedReader(fileReader);
        try {
            while ((line=bufferedReader.readLine())!=null) {
                String[] info = getInfo(line);
                //判断info的信息是否与Road的信息一样多，如果是则填充Road对象并加到list里
                if (Car.numberPara==info.length) {
                    Car car = new Car();
                    car.setId(Integer.parseInt(info[0]));
                    car.setBeginId(Integer.parseInt(info[1]));
                    car.setEndId(Integer.parseInt(info[2]));
                    car.setSpeedLimit(Integer.parseInt(info[3]));
                    car.setDepartTime(Integer.parseInt(info[4]));
                    list.add(car);
                }
            }
            return list;
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            //关闭资源
            FileUtil.closeBufferedReader(bufferedReader);
        }
        return list;
    }

    static public List<Cross> readCross (String path) {
        List<Cross> list = new ArrayList<>();
        String line;
        FileReader fileReader = FileUtil.getFileReader(path);
        BufferedReader bufferedReader = FileUtil.getBufferedReader(fileReader);
        try {
            while ((line=bufferedReader.readLine())!=null) {
                String[] info = getInfo(line);
                //判断info的信息是否与Road的信息一样多，如果是则填充Road对象并加到list里
                if (Cross.numberPara==info.length) {
                    Cross cross = new Cross();
                    cross.setId(Integer.parseInt(info[0]));
                    cross.setRoadId1(Integer.parseInt(info[1]));
                    cross.setRoadId2(Integer.parseInt(info[2]));
                    cross.setRoadId3(Integer.parseInt(info[3]));
                    cross.setRoadId4(Integer.parseInt(info[4]));
                    List<Integer> roadIdList = new LinkedList<>();
                    roadIdList.add(cross.getRoadId1());
                    roadIdList.add(cross.getRoadId2());
                    roadIdList.add(cross.getRoadId3());
                    roadIdList.add(cross.getRoadId4());
                    cross.setRoadIdList(roadIdList);
                    list.add(cross);
                }
            }
            return list;
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            //关闭资源
            FileUtil.closeBufferedReader(bufferedReader);
        }
        return list;
    }

    static private String[] getInfo(String line) {
        String[] info = new String[]{};
        //判断是否是一个有效的输入，如果是，则进行字符串切分。得到info字符串数组。
        if (isValid(line)) {
            line = line.substring(1, line.length()-1);
            //info = line.split("\\s*,\\s+|,");   //这里的正则表达式需要注意，\\s+为匹配多个(大于一个)空格，|表示或者后面这种情况
            info = line.replaceAll("\\s*","").split(",");   //上面的代码仍然在第一个数和最后有bug，建议写成下面这种形式
        }
        return info;
    }

    static private boolean isValid(String line) {
        if (line!=null && line.length()>=2 && line.charAt(0)=='(' && line.charAt(line.length()-1)==')') {
            return true;
        }
        return false;
    }
}
