import java.util.ArrayList;

public class Dungeon {
    public static final int MAX_FLOOR = 30;
    public static final int ROOMS_PER_FLOOR = 5;
    public static final int BOSS_FLOOR_INTERVAL = 10;

    private int currentFloor;
    private int currentRoom;
    private Player player;
    private ArrayList<Monster> monstersCleared;

    public Dungeon(Player player) {
        this.player = player;
        this.currentFloor = 1;
        this.currentRoom = 1;
        this.monstersCleared = new ArrayList<>();
    }

    // done: 현재 위치에 맞는 몬스터를 생성하는 메서드를 구현하세요
    // 보스 방이면 BossMonster.createBossForFloor를, 아니면 NormalMonster.createRandomMonster를 호출합니다
    public Monster generateMonster() {
        if (isBossRoom()) {
            return BossMonster.createBossForFloor(currentFloor);
        } else {
            return NormalMonster.createRandomMonster(currentFloor);
        }
    }

    // 다음 방으로 이동하는 메서드
    // 1. 현재 방 번호를 1 증가시킵니다
    // 2. 방 번호가 ROOMS_PER_FLOOR보다 크면 moveToNextFloor()를 호출합니다
    // 3. 아니면 "다음 방으로 이동합니다. 현재 위치: X층 Y번 방"형식으로 출력합니다 (X: 현재 층, Y: 다음 방)
    public void moveToNextRoom() {
        currentRoom += 1;
        if (currentRoom > ROOMS_PER_FLOOR) moveToNextFloor();
        else System.out.println("다음 방으로 이동합니다. 현재 위치: " + currentFloor + "층 " + currentRoom + "번 방");
    }

    // 다음 층으로 이동하는 메서드
    // 1. 현재 층 번호를 1 증가시킵니다
    // 2. 현재 방 번호를 1로 설정합니다
    // 3. 플레이어의 체력을 완전히 회복시킵니다
    // 4. 던전이 클리어되지 않았으면 "다음 층으로 이동합니다. 현재 위치: X층 Y번 방"형식으로 출력합니다 (X: 다음 층, Y: 1)
    public void moveToNextFloor() {
        currentFloor += 1;
        currentRoom = 1;
        player.recoverFullHealth();

        if (!isDungeonCleared()) System.out.println("다음 층으로 이동합니다. 현재 위치: " + currentFloor + "층 " + currentRoom + "번 방" );
    }

    // 던전 클리어 여부를 확인하는 메서드
    // 현재 층이 MAX_FLOOR보다 크면 true, 아니면 false를 반환합니다
    public boolean isDungeonCleared() {
        if (currentFloor > MAX_FLOOR) return true;
        else return false;
    }

    // 현재 층이 보스 층인지 확인하는 메서드
    // 현재 층이 BOSS_FLOOR_INTERVAL의 배수이면 true, 아니면 false를 반환합니다
    public boolean isBossFloor() {
        if (currentFloor % BOSS_FLOOR_INTERVAL == 0) return true;
        else return false;
    }

    // 현재 방이 보스 방인지 확인하는 메서드
    // 현재 층이 보스 층이고 현재 방이 ROOMS_PER_FLOOR와 같으면 true, 아니면 false를 반환합니다
    public boolean isBossRoom() {
        if (isBossFloor() && currentRoom == ROOMS_PER_FLOOR) return true;
        else return false;
    }

    // 현재 위치를 문자열로 반환하는 메서드
    // "X층 Y번 방" 형식으로 반환합니다(X: 현재 층, Y: 현재 방)
    public String getCurrentLocation() {
        return currentFloor + "층 " + currentRoom + "번 방";
    }

    // Getter와 Setter 메서드들
    public int getCurrentFloor() {
        return currentFloor;
    }

    public void setCurrentFloor(int currentFloor) {
        this.currentFloor = currentFloor;
    }

    public int getCurrentRoom() {
        return currentRoom;
    }

    public void setCurrentRoom(int currentRoom) {
        this.currentRoom = currentRoom;
    }

    public Player getPlayer() {
        return player;
    }

    public void setPlayer(Player player) {
        this.player = player;
    }

    public ArrayList<Monster> getMonstersCleared() {
        return monstersCleared;
    }

    public void setMonstersCleared(ArrayList<Monster> monstersCleared) {
        this.monstersCleared = monstersCleared;
    }
}