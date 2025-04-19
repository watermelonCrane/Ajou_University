public abstract class Item {
    private String name;
    private int floor;

    public Item(String name, int floor) {
        this.name = name;
        this.floor = floor;
    }

    // 아이템 설명을 반환하는 추상 메서드 (각 하위 클래스에서 구현해야 함)
    public abstract String getDescription();

    // Getter와 Setter 메서드들
    public String getName() {
        return name;
    }

    protected void setName(String name) {
        this.name = name;
    }

    public int getFloor() {
        return floor;
    }

    protected void setFloor(int floor) {
        this.floor = floor;
    }
}