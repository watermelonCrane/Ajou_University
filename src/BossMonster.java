public class BossMonster extends Monster {

    private static final String[][] BOSS_DATA = {
            {"슬라임 킹", "80", "12", "50"},
            {"오크 킹", "150", "20", "100"},
            {"골드 드래곤", "250", "30", "200"}
    };

    public BossMonster(String name, int level, int floor, int maxHP, int attackPower, int expReward) {
        super(name, level, floor, maxHP, attackPower, expReward);
    }

    // Done: 층에 맞는 보스 몬스터정보를 위의 BOSS_DATA에서 가져와 생성하는 정적 메서드를 구현하세요
    // 1. 입력받은 floor를 기반으로 적절한 보스 인덱스를 계산합니다 ((floor / 10) - 1)
    // 2. 인덱스가 BOSS_DATA 배열 범위를 벗어나면 마지막 보스 데이터를 사용합니다
    // 3. 선택된 보스 데이터로부터 이름, 최대 체력, 공격력, 경험치 보상을 가져옵니다
    // 4. 레벨은 (floor / 2 + 5)로 계산합니다
    // 5. 새로운 BossMonster 객체를 생성하여 반환합니다
    public static BossMonster createBossForFloor(int floor) {
        // 1, 2
        int index = Math.min(((floor / 10) - 1), 2);    // 인덱스는 2보다 작아야함.

        // 3, 4
        String name = BOSS_DATA[index][0];
        int hp = Integer.parseInt(BOSS_DATA[index][1]);
        int atk = Integer.parseInt(BOSS_DATA[index][2]);
        int exp = Integer.parseInt(BOSS_DATA[index][3]);

        int level = floor / 2 + 5;

        // 5
        BossMonster boss = new BossMonster(name, level, floor, hp, atk, exp);
        return boss;
    }

    // done: 보스 몬스터가 드롭하는 아이템을 구현하세요
    // 1. 층 수가 20이면 방어구를, 아니면 무기를 드롭합니다
    // 2. 방어구는 "(floor)층 보스의 방어구" 이름에 방어력이 (floor/2)인 Armor 객체입니다
    // 3. 무기는 "(floor)층 보스의 무기" 이름에 공격력이 floor인 Weapon 객체입니다
    @Override
    public Item dropItem() {
        int floor = getFloor();
        Item item;
        if (floor == 20) {
            item = new Armor(floor + "층 보스의 방어구", floor, floor / 2);
        } else {
            item = new Weapon(floor + "층 보스의 무기", floor, floor);
        }
        return item;
    }

    // done: 보스 몬스터 정보를 문자열로 반환하는 메서드를 오버라이드하세요
    // "[보스] S" 형식으로 반환합니다 (S: 보스이름)
    @Override
    public String getInfo() {
        return "[보스] " + getName(); // 이 부분을 수정하세요
    }
}