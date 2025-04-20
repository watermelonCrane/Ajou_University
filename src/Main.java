/*todo: 질문할 것들
* Monster, Player 클래스의 calculateDamage 계산 방식 질문
* * 레벨차이가 절대값인지, 절대값이 아니라면 몬스터 레벨이 더 높을 때 배율 적용인지.
* * 최소 배율 0.1이 3번 전체를 이야기하는 건지(말이 안되긴 함). 아니면 '레벨차이 * 0.1' <- 여기만 말하는건지
*
* Monster 클래스의 attack 메소드에서 치명타일 경우 1.5배 데미지 증가인데, int형으로 형변환 시켜야 하는지(healthPoints는 int니까.). 그렇다면, 결국엔 정수배로 증가되는게 아닌지.
*
* Dungeon 클래스의 inDungeonCleared 메소드가 완성이 되어있는 것 같은데, todo는 무시해도 되는 것인지?
*
* */

/* todo: 구현 해야할 것들
* Entity - done
* Monster - done
* NormalMonster - done
* BossMonster - FIXME
* Player - done
*
* Item - done 하위 클래스 구현 확인필요
* Armor - done
* Weapon - done
*
* Dungeon - done
*
*
* todo: 버그 픽스 필요한 것들
*  1. 게임이 클리어되어도 종료가 안됨. Game 클래스에 표시해놨음 그 부분 살펴보자.
*
* */

import java.io.*;
import java.util.*;

public class Main {
    public static void main(String[] args) {
        try {
            System.setOut(new PrintStream(System.out, true, "UTF-8")); //UTF-8 인코딩 사용
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }

        System.out.println("30층 던전 탐험 게임에 오신 것을 환영합니다!");
        System.out.println("플레이어 이름을 입력하세요:");

        Scanner scanner = new Scanner(System.in, "UTF-8"); //UTF-8 인코딩 사용
        String playerName = scanner.nextLine();

        Game game = new Game(playerName);
        game.start();

        scanner.close();
    }
}