public abstract class Monster extends Entity implements Fight {
    private int attackPower;
    private int expReward;
    private int floor;

    public Monster(String name, int level, int floor, int maxHP, int attackPower, int expReward) {
        super(name, level, maxHP);
        this.attackPower = attackPower;
        this.expReward = expReward;
        this.floor = floor;
    }

    @Override
    public int getBaseDamage() {
        return attackPower;
    }

    public void setAttackPower(int attackPower) {
        this.attackPower = attackPower;
    }

    // TODO: 몬스터의 공격 메서드를 구현하세요
    // 1. calculateDamage 메서드를 호출하여 데미지를 계산합니다
    // 2. RandomManager.isCritical()을 사용하여 치명타 여부를 확인합니다
    // 3. 치명타인 경우 데미지를 1.5배로 증가시키고 "몬스터의 치명타!" 메시지를 출력합니다
    // 4. 공격 메시지 "S가 X의 데미지를 입혔습니다."를 출력합니다 (S: 몬스터 이름, X: 데미지 수치)
    // 5. target의 takeDamage 메서드를 호출하여 데미지를 입힙니다
    // 6. 최종 데미지를 반환합니다
    @Override
    public int attack(Entity target) {
        int damage = calculateDamage(target);       // 1
        if (RandomManager.isCritical()) {
            damage *= 1.5;
            System.out.println("몬스터의 치명타!");
        }   // 2, 3

        System.out.println(getName() + "가 " + damage + "의 데미지를 입혔습니다."); // 4

        target.takeDamage(damage); // 5

        return damage; // 6
    }

    // TODO: 몬스터의 데미지 계산 메서드를 구현하세요
    // 1. getBaseDamage 메서드를 호출하여 기본 데미지를 가져옵니다
    // 2. 레벨 차이에 따른 데미지 배율을 계산합니다 (레벨 차이 * 0.1 + 1.0)
    // 3. 배율은 최소 0.1 이상이 되어야 합니다 (Math.max 활용)
    // 4. 기본 데미지에 배율을 곱하여 최종 데미지를 계산하고 반환합니다 (int로 형변환)
    @Override
    public int calculateDamage(Entity target) {
        int baseDamage = getBaseDamage();   // 1
        int levelGap = target.getLevel() - getLevel();
        double damageMag = Math.max(levelGap * 0.1, 0.1) + 1.0;     //2, 3

        return (int) (baseDamage * damageMag);      // 4
    }

    // DONE: 몬스터 정보를 문자열로 반환하는 메서드를 구현하세요
    // "S (레벨 X) - HP: Y/Z" 형식으로 반환합니다(S: 몬스터 이름, X: 몬스터 레벨, Y: 현재 체력, Z: 전체 체력)
    public String getInfo() {
        return getName() + " (레벨 " + getLevel() + ") - HP: " + getHealthPoints() + "/" + getMaxHealthPoints();
    }

    // 몬스터가 드롭하는 아이템을 반환하는 추상 메서드 (각 하위 클래스에서 구현해야 함)
    public abstract Item dropItem();

    public int getExpReward() {
        return expReward;
    }

    protected void setExpReward(int expReward) {
        this.expReward = expReward;
    }

    public int getFloor() {
        return floor;
    }

    protected void setFloor(int floor) {
        this.floor = floor;
    }
}