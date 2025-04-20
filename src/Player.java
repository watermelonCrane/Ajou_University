import java.util.ArrayList;

public class Player extends Entity implements Fight {
    private int experiencePoints;
    private ArrayList<Item> inventory;
    private Weapon equippedWeapon;
    private Armor equippedArmor;
    private int totalKills;

    private static final Weapon BEGINNER_WEAPON = new Weapon("초보자의 검", 1, 3);
    private static final Armor BEGINNER_ARMOR = new Armor("초보자의 방어구", 1, 2);

    public Player(String name) {
        super(name, 1, 100);
        this.experiencePoints = 0;
        this.inventory = new ArrayList<>();

        this.equippedWeapon = BEGINNER_WEAPON;
        this.equippedArmor = BEGINNER_ARMOR;

        this.inventory.add(BEGINNER_WEAPON);
        this.inventory.add(BEGINNER_ARMOR);

        this.totalKills = 0;
    }

    // done: 경험치 획득 메서드를 구현하세요
    // 1. 경험치를 매개변수만큼 증가시킵니다
    // 2. 경험치 획득 메시지 "S가 X 경험치를 획득했습니다."를 출력합니다 (S: 플레이어 이름, X: 경험치 양)
    // 3. 경험치가 100 이상이면 반복해서 levelUp 메서드를 호출합니다
    public void gainExperience(int exp) {
        // 1    setter를 써야하나 고민중
        experiencePoints += exp;
        System.out.println(getName() + "가 " + exp + " 경험치를 획득했습니다.");   // 2
        while (experiencePoints >= 100) levelUp();       // 3
    }

    // done: 레벨업 메서드를 구현하세요
    // 1. 레벨을 1 증가시킵니다
    // 2. 경험치를 100 감소시킵니다
    // 3. 최대 체력을 20 증가시킵니다
    // 4. 현재 체력을 최대 체력으로 회복시킵니다
    // 5. 레벨업 메시지 "S의 레벨이 X로 올랐습니다!"를 출력합니다(S: 플레이어 이름, X: 플레이어 레벨)
    // 6. 체력 회복 메시지 "체력이 완전히 회복되었습니다!"를 출력합니다
    public void levelUp() {
        setLevel(getLevel() + 1);   // 1
        setExperiencePoints(getExperiencePoints() - 100); // 2
        // experiencePoints -= 100;    // 2 위에것이랑 이거랑 뭐가 더 좋은가?
        setMaxHealthPoints(getMaxHealthPoints() + 20);  // 3
        setHealthPoints(getMaxHealthPoints());  // 4
        System.out.println(getName() + "의 레벨이 " + getLevel() + "로 올랐습니다!");
        System.out.println("체력이 완전히 회복되었습니다.!");
    }

    // done: 아이템 장착 메서드를 구현하세요
    // 1. 아이템이 Weapon인지 Armor인지 확인합니다 (instanceof 활용)
    // 2. Weapon이면 equippedWeapon에, Armor면 equippedArmor에 장착합니다
    // 3. 아이템 장착 메시지 "S을(를) 장착했습니다."를 출력합니다 (S: 장비 이름)
    public void equipItem(Item item) {
        if (item instanceof Weapon) setEquippedWeapon((Weapon) item);
        else setEquippedArmor((Armor) item);

        System.out.println(item.getName() + "을(를) 장착했습니다.");
    }

    @Override
    public int getBaseDamage() {
        return 5;
    }

    // done: 플레이어 공격 메서드를 구현하세요
    // 1. calculateDamage 메서드를 호출하여 데미지를 계산합니다
    // 2. RandomManager.isCritical()을 사용하여 치명타 여부를 확인합니다
    // 3. 치명타인 경우 데미지를 2배로 증가시키고 "치명타!" 메시지를 출력합니다
    // 4. 공격 메시지 "S가 X의 데미지를 입혔습니다."를 출력합니다 (S: 플레이어 이름, X: 데이지 수치)
    // 5. target의 takeDamage 메서드를 호출하여 데미지를 입힙니다
    // 6. 최종 데미지를 반환합니다
    @Override
    public int attack(Entity target) {
        int damage = calculateDamage(target); // 1
        if (RandomManager.isCritical()) {
            damage *= 2;
            System.out.println("치명타!");
        }   // 2, 3

        System.out.println(getName() + "가 " + damage + "의 데미지를 입혔습니다.");    //4
        target.takeDamage(damage);  // 5

        return damage;
    }

    // FIXME: 플레이어의 데미지 계산 메서드를 구현하세요
    // 1. getBaseDamage 메서드를 호출하여 기본 데미지를 가져옵니다
    // 2. 무기가 장착되어 있으면 무기의 데미지를 추가합니다
    // 3. 레벨 차이에 따른 데미지 배율을 계산합니다 (레벨 차이 * 0.1 + 1.0)
    // 4. 배율은 최소 0.1 이상이 되어야 합니다 (Math.max 활용)
    // 5. 기본 데미지에 배율을 곱하여 최종 데미지를 계산하고 반환합니다 (int로 형변환)
    @Override
    public int calculateDamage(Entity target) {
        int damage = getBaseDamage(); // 1
        if (equippedWeapon != null) damage += equippedWeapon.getDamage();   // 2
        int levelGap =Math.abs(target.getLevel() - getLevel()); // FIXME, 레벨차이의 정의 질문 답변 본 후 다시 작성
        double damageMag = Math.max(levelGap, 0.1) + 1.0;   //3, 4

        return (int) (damage * damageMag);  // 5
    }

    // done: 플레이어가 데미지를 받는 메서드를 오버라이드하여 구현하세요
    // 1. 방어구가 장착되어 있으면 방어구의 방어력만큼 데미지를 감소시킵니다 (최소 1)
    // 2. 방어 메시지 "S이(가) 데미지를 X 감소시켰습니다."를 출력합니다 (S: 방어구 이름, X: 감소된 데미지 양)
    // 3. 감소된 데미지로 부모 클래스의 takeDamage 메서드를 호출합니다
    // 4. 플레이어가 사망한 경우 사망 메시지 "S가 사망했습니다."를 출력합니다 (S: 플레이어 이름)
    @Override
    public void takeDamage(int damage) {
        if (equippedArmor != null) {
            int deDamage = Math.max(equippedArmor.getDefense(), 1);
            damage -= deDamage; // 1

            System.out.println(equippedArmor.getName() + "이(가) 데미지를 " + deDamage + "감소시켰습니다.");     //2
        }

        super.takeDamage(damage);   //3

        if (!isAlive()) System.out.println(getName() + "가 사망했습니다.");    //  4
    }

    // done: 인벤토리에 아이템을 추가하는 메서드를 구현하세요
    // 1. 인벤토리에 아이템을 추가합니다
    // 2. 아이템 획득 메시지 "S을(를) 획득했습니다."를 출력합니다 (S: 아이템 이름)
    public void addToInventory(Item item) {
        inventory.add(item);
        System.out.println(item.getName() + "을(를) 획득했습니다.");
    }

    // done: 전체 처치한 몬스터 수를 1 증가시키는 메서드를 구현하세요
    public void incrementKills() {
        setTotalKills(getTotalKills() + 1);
    }

    // done: 체력을 완전히 회복시키는 메서드를 구현하세요
    // 1. 현재 체력을 최대 체력으로 설정합니다
    // 2. 체력 회복 메시지 "S의 체력이 완전히 회복되었습니다!"를 출력합니다 (S: 플레이어의 이름)
    public void recoverFullHealth() {
        setHealthPoints(getMaxHealthPoints());
        System.out.println(getName() + "의 체력이 완전히 회복되었습니다!");
    }

    // 플레이어 상태를 출력하는 메서드
    public void printStatus() {
        System.out.println("\n===== 플레이어 상태 =====");
        System.out.println("이름: " + getName());
        System.out.println("레벨: " + getLevel());
        System.out.println("경험치: " + experiencePoints + "/100");
        System.out.println("체력: " + getHealthPoints() + "/" + getMaxHealthPoints());
        System.out.println("무기: " + (equippedWeapon != null ? equippedWeapon.getDescription() : "없음"));
        System.out.println("방어구: " + (equippedArmor != null ? equippedArmor.getDescription() : "없음"));
        System.out.println("처치한 몬스터: " + totalKills);
        System.out.println("=========================");
    }

    // done: 인벤토리를 출력하는 메서드를 구현하세요
    // 다른 텍스트와 구별을 위해 "\n===== 인벤토리 ====="를 출력합니다.
    // 인벤토리가 비어있으면 "인벤토리가 비어있습니다."를 출력하고,
    // 아이템이 있으면 각 아이템의 번호와 설명을 출력합니다
    // 마지막으로 "==================="를 출력합니다.
    public void printInventory() {
        System.out.println("\\n===== 인벤토리 =====");
        if (inventory.isEmpty()) {
            System.out.println("인벤토리가 비어있습니다.");
        }
        else {
            for (int i = 0; i < inventory.size(); i++) {
                System.out.printf("%d: ", i + 1);
                System.out.println(inventory.get(i).getDescription());
            }
        }
        System.out.println("===================");
    }

    // Getter와 Setter 메서드들
    public int getExperiencePoints() {
        return experiencePoints;
    }

    protected void setExperiencePoints(int experiencePoints) {
        this.experiencePoints = experiencePoints;
    }

    public ArrayList<Item> getInventory() {
        return inventory;
    }

    public Weapon getEquippedWeapon() {
        return equippedWeapon;
    }

    protected void setEquippedWeapon(Weapon weapon) {
        this.equippedWeapon = weapon;
    }

    public Armor getEquippedArmor() {
        return equippedArmor;
    }

    protected void setEquippedArmor(Armor armor) {
        this.equippedArmor = armor;
    }

    public int getTotalKills() {
        return totalKills;
    }

    protected void setTotalKills(int totalKills) {
        this.totalKills = totalKills;
    }
}