public abstract class Entity {
    private String name;
    private int healthPoints;
    private int maxHealthPoints;
    private int level;

    public Entity(String name, int level, int maxHealthPoints) {
        this.name = name;
        this.level = level;
        this.maxHealthPoints = maxHealthPoints;
        this.healthPoints = maxHealthPoints;
    }

    // DONE: 데미지를 받는 메서드를 구현하세요
    // 매개변수로 받은 damage만큼 healthPoints를 감소시킵니다
    // 단, healthPoints는 0보다 작아질 수 없습니다 (Math.max 활용)
    // 데미지를 받은 후 남은 체력을 "S가 X의 데미지를 받았습니다. 남은 체력: Y"를 출력합니다(S: 이름, X: 데미지, Y: HP)
    public void takeDamage(int damage) {
        healthPoints = Math.max(healthPoints - damage, 0);  // healthPoints - damage가 0보다 클경우에만, 작으면 0으로 설정
        System.out.println(name + "가 " + damage + "의 데미지를 받았습니다. 남은 체력: " + healthPoints);
    }

    public boolean isAlive() {
        return healthPoints > 0;    // healthPoints가 0보다 크면 true 반환, 아니면 false 반환
    }

    // 기본 공격력을 반환하는 추상 메서드 (각 하위 클래스에서 구현해야 함)
    public abstract int getBaseDamage();

    // Getter와 Setter 메서드들
    public String getName() {
        return name;
    }

    protected void setName(String name) {
        this.name = name;
    }

    public int getHealthPoints() {
        return healthPoints;
    }

    protected void setHealthPoints(int healthPoints) {
        this.healthPoints = healthPoints;
    }

    public int getMaxHealthPoints() {
        return maxHealthPoints;
    }

    protected void setMaxHealthPoints(int maxHealthPoints) {
        this.maxHealthPoints = maxHealthPoints;
    }

    public int getLevel() {
        return level;
    }

    public void setLevel(int level) {
        this.level = level;
    }
}