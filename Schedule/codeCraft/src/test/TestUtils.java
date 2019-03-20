package test;

import org.junit.jupiter.api.Test;
import pojo.Car;
import pojo.Cross;
import pojo.Road;
import utils.ReadData;

import java.io.IOException;
import java.util.List;

public class TestUtils {
    List<Road> list;

    @Test
    public void testReadRoad() {
        List<Road> listroad;
        listroad = ReadData.readRoad("data/Roads.txt");
        System.out.println(listroad);

    }

    @Test
    public void testReadCar() {
        List<Car> listCar;
        listCar = ReadData.readCar("data/Car.txt");
        System.out.println(listCar);
    }

    @Test
    public void testReadCross() {
        List<Cross> listCross;
        listCross = ReadData.readCross("data/Cross.txt");
        System.out.println(listCross);
    }
}
