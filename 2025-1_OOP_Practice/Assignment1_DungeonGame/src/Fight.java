public interface Fight {
    // 대상을 공격하여 데미지를 입히는 메서드
    // 반환값은 입힌 데미지입니다
    int attack(Entity target);

    // 대상에게 입힐 데미지를 계산하는 메서드
    // 레벨 차이, 장비 등을 고려하여 최종 데미지를 계산합니다
    int calculateDamage(Entity target);
}