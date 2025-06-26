public class NormalMonster extends Monster {

    public NormalMonster(String name, int level, int floor) {
        super(name, level, floor,
                calculateBaseHP(name, level),
                calculateBaseAttack(name, level),
                calculateBaseExp(name, level));
    }

    // 층에 맞는 일반 몬스터를 무작위로 생성하는 정적 메서드
    // 1. 층 수에 따라 사용 가능한 몬스터 이름 배열을 설정합니다
    //    - 1~10층: "슬라임", "고블린"
    //    - 11~20층: "고블린", "오크"
    //    - 21~30층: "오크", "해골", "좀비"
    // 2. RandomManager.randomFromZeroTo를 사용하여 무작위 이름을 선택합니다
    // 3. 레벨은 (floor / 2 + RandomManager.randomFromZeroTo(2))로 계산합니다
    // 4. 새로운 NormalMonster 객체를 생성하여 반환합니다
    public static NormalMonster createRandomMonster(int floor) {
        String[] possibleNames;
        // 1
        if (floor <= 10)
            possibleNames = new String[]{"슬라임", "고블린"};
        else if (floor <= 20)
            possibleNames = new String[]{"고블린", "오크"};
        else
            possibleNames = new String[]{"오크", "해골", "좀비"};

        // 2
        String monsterName = possibleNames[RandomManager.randomFromZeroTo(possibleNames.length - 1)];

        // 3
        int monsterLevel = (floor / 2 + RandomManager.randomFromZeroTo(2));

        // 4
        NormalMonster monster = new NormalMonster(monsterName, monsterLevel, floor);
        return monster;
    }

    // 몬스터 이름과 레벨에 따른 기본 체력을 계산하는 정적 메서드
    // 1. 몬스터 이름에 따라 기본 체력을 설정합니다:
    //    - "슬라임": 20
    //    - "고블린": 35
    //    - "오크": 50
    //    - "해골": 40
    //    - "좀비": 45
    //    - 기타: 30
    // 2. 레벨에 따라 추가 체력을 계산합니다: 기본 체력 + (레벨 - 1) * 5
    private static int calculateBaseHP(String name, int level) {
        // 1
        int basicHP = switch (name) {
            case "슬라임" -> 20;
            case "고블린" -> 35;
            case "오크" -> 50;
            case "해골" -> 40;
            case "좀비" -> 45;
            default -> 30;
        };

        // 2
        return basicHP + (level - 1) * 5;
    }

    // 몬스터 이름과 레벨에 따른 기본 공격력을 계산하는 정적 메서드
    // 1. 몬스터 이름에 따라 기본 공격력을 설정합니다:
    //    - "슬라임": 3
    //    - "고블린": 5
    //    - "오크": 8
    //    - "해골": 6
    //    - "좀비": 7
    //    - 기타: 4
    // 2. 레벨에 따라 추가 공격력을 계산합니다: 기본 공격력 + (레벨 - 1) * 2
    private static int calculateBaseAttack(String name, int level) {
        // 1
        int basicAttack = switch (name) {
            case "슬라임" -> 3;
            case "고블린" -> 5;
            case "오크" -> 8;
            case "해골" -> 6;
            case "좀비" -> 7;
            default -> 4;
        };

        // 2
        return basicAttack + (level - 1) * 2;
    }

    // 몬스터 이름과 레벨에 따른 기본 경험치를 계산하는 정적 메서드
    // 1. 몬스터 이름에 따라 기본 경험치를 설정합니다:
    //    - "슬라임": 10
    //    - "고블린": 15
    //    - "오크": 25
    //    - "해골": 20
    //    - "좀비": 22
    //    - 기타: 12
    // 2. 레벨에 따라 추가 경험치를 계산합니다: 기본 경험치 + (레벨 - 1) * 3
    private static int calculateBaseExp(String name, int level) {
        // 1
        int basicExp = switch (name) {
            case "슬라임" -> 10;
            case "고블린" -> 15;
            case "오크" -> 25;
            case "해골" -> 20;
            case "좀비" -> 22;
            default -> 12;
        };

        // 2
        return basicExp + (level - 1) * 3;
    }

    // 일반 몬스터는 아이템을 드롭하지 않습니다
    @Override
    public Item dropItem() {
        return null;
    }
}