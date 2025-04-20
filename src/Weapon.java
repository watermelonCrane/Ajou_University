public class Weapon extends Item {
    private int damage;

    public Weapon(String name, int floor, int damage) {
        super(name, floor);
        this.damage = damage;
    }

    public int getDamage() {
        return damage;
    }

    protected void setDamage(int damage) {
        this.damage = damage;
    }

    // Done: 무기 설명을 반환하는 메서드를 구현하세요
    // "S (공격력: X, Y층)" 형식으로 반환합니다 (S: 장비이름, X: 공격력 수치, Y: 획득한 층)
    @Override
    public String getDescription() {
        return getName() + " (공격력: " + damage + ", " + getFloor() + "층)";
    }
}