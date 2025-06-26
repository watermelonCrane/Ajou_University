import java.util.Random;

public class RandomManager {
    private static final Random random = new Random();

    // 30% 확률로 치명타 여부를 반환하는 메서드
    public static boolean isCritical() {
        return random.nextInt(100) < 30;
    }

    // 0부터 max까지의 무작위 정수를 반환하는 메서드
    public static int randomFromZeroTo(int max) {
        return random.nextInt(max + 1);
    }

    // min부터 max까지의 무작위 정수를 반환하는 메서드
    public static int randomBetween(int min, int max) {
        return min + random.nextInt(max - min + 1);
    }

    // 지정된 확률(0-100)로 성공 여부를 반환하는 메서드
    public static boolean randomChance(int probability) {
        return random.nextInt(100) < probability;
    }
}