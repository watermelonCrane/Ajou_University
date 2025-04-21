/* todo: 구현 해야할 것들
* 1. 전체적으로 확인
* 2. 메일 오면 내용 확인 후 수정
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