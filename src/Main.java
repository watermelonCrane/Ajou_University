/*todo: 질문할 것들
* Monster, Player 클래스의 calculateDamage 계산 방식 질문
* * 레벨차이가 절대값인지, 절대값이 아니라면 몬스터 레벨이 더 높을 때 배율 적용인지.
* * 최소 배율 0.1이 3번 전체를 이야기하는 건지(말이 안되긴 함).
*
* Monster 클래스의 attack 메소드에서 치명타일 경우 1.5배 데미지 증가인데, int형으로 형변환 시켜야 하는지(healthPoints는 int니까.). 그렇다면, 결국엔 정수배로 증가되는게 아닌지.
*
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