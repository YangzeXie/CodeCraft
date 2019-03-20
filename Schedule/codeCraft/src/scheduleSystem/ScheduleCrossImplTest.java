package scheduleSystem;

import org.junit.jupiter.api.Test;

import java.util.PriorityQueue;

class ScheduleCrossImplTest {

    @Test
    void scheduleOneCross() {
    }

    @Test
    void scheduleAllCross() {
    }

    @Test
    void testPriq() {
        PriorityQueue<Integer> p = new PriorityQueue<>();
        p.add(4);
        p.add(2);
        p.add(15);
        for (int num : p) {
            System.out.printf("%d ", num);
        }
    }
}