public class Armor extends Item {
    private int defense;

    public Armor(String name, int floor, int defense) {
        super(name, floor);
        this.defense = defense;
    }

    public int getDefense() {
        return defense;
    }

    protected void setDefense(int defense) {
        this.defense = defense;
    }

    // Done: 방어구 설명을 반환하는 메서드를 구현하세요
    // "S (방어력: X, Y층)" 형식으로 반환합니다 (S: 장비이름, X: 방어력 수치, Y: 획득한 층)
    @Override
    public String getDescription() {
        return getName() + " (방어력: " + defense + ", " + getFloor() + "층";
    }
}