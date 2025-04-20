import java.util.ArrayList;
import java.util.Scanner;

public class Game {
    private Player player;
    private Dungeon dungeon;
    private Scanner scanner;

    public Game(String playerName) {
        this.player = new Player(playerName);
        this.dungeon = new Dungeon(player);
        this.scanner = new Scanner(System.in);
    }

    public void start() {
        System.out.println("===== 30층 던전 탐험 게임을 시작합니다 =====");
        System.out.println(player.getName() + "님, 던전 탐험을 시작합니다.");
        System.out.println("10층, 20층, 30층마다 보스가 있고, 30층 보스를 물리치면 게임 클리어입니다.");

        boolean isRunning = true;

        while (isRunning) {
            if (dungeon.isDungeonCleared()) {
                gameCleared();
                break;
            }

            if (!player.isAlive()) {
                gameOver();
                break;
            }

            displayGameMenu();
            int choice = getPlayerChoice(1, 4);

            switch (choice) {
                case 1:
                    exploreRoom();
                    break;
                case 2:
                    player.printStatus();
                    break;
                case 3:
                    player.printInventory();
                    equipItemFromInventory();
                    break;
                case 4:
                    System.out.println("게임을 종료합니다.");
                    isRunning = false;
                    break;
            }
        }

        scanner.close();
    }

    private void displayGameMenu() {
        System.out.println("\n===== 현재 위치: " + dungeon.getCurrentLocation() + " =====");
        System.out.println("1. 탐험하기");
        System.out.println("2. 상태 확인");
        System.out.println("3. 인벤토리 확인");
        System.out.println("4. 게임 종료");
        System.out.print("선택: ");
    }

    private void exploreRoom() {
        System.out.println("\n===== " + dungeon.getCurrentLocation() + " 탐험 =====");

        if (dungeon.isBossRoom()) {
            System.out.println("보스 방에 들어섰습니다!");
            Monster boss = dungeon.generateMonster();
            System.out.println(boss.getInfo() + "가 나타났습니다!");

            boolean battleResult = battle(boss);

            if (battleResult && boss instanceof BossMonster) {
                Item bossItem = boss.dropItem();
                if (bossItem != null) {
                    player.addToInventory(bossItem);
                    System.out.println("보스 아이템을 획득했습니다: " + bossItem.getDescription());

                    System.out.print("새로운 아이템을 장착하시겠습니까? (y/n): ");
                    String answer = scanner.next();
                    if (answer.equalsIgnoreCase("y")) {
                        player.equipItem(bossItem);
                    }
                }

                if (dungeon.getCurrentFloor() == Dungeon.MAX_FLOOR) {
                    return; //FIXME
                }
            }
        }
        else if (RandomManager.randomChance(70)) {
            Monster monster = dungeon.generateMonster();
            System.out.println(monster.getInfo() + "가 나타났습니다!");
            battle(monster);
        } else {
            System.out.println("이 방에는 아무것도 없는 것 같습니다.");
        }

        if (player.isAlive()) {
            handleMovementAfterExploration();
        }
    }

    private void handleMovementAfterExploration() {
        boolean decided = false;

        while (!decided) {
            displayMoveOptions();
            int choice = getPlayerChoice(1, 3);

            switch (choice) {
                case 1:
                    dungeon.moveToNextRoom();
                    decided = true;
                    break;
                case 2:
                    player.printStatus();
                    break;
                case 3:
                    player.printInventory();
                    equipItemFromInventory();
                    break;
            }
        }
    }

    private void displayMoveOptions() {
        System.out.println("\n다음 행동을 선택하세요:");
        System.out.println("1. 다음 방으로 이동");
        System.out.println("2. 상태 확인");
        System.out.println("3. 인벤토리 확인");
        System.out.print("선택: ");
    }

    private boolean battle(Monster monster) {
        System.out.println("\n===== 전투 준비 =====");
        System.out.println("1. 전투를 한다.");
        System.out.println("2. 도망간다. (30% 확률로 성공)");
        System.out.print("선택: ");

        int choice = getPlayerChoice(1, 2);

        if (choice == 2) {
            if (RandomManager.randomChance(30)) {
                System.out.println("도망치는데 성공했습니다!");
                return false;
            } else {
                System.out.println("도망치는데 실패했습니다! 전투를 시작합니다.");
            }
        }

        System.out.println("\n===== 전투 시작 =====");

        while (player.isAlive() && monster.isAlive()) {
            System.out.println("\n" + player.getName() + "의 턴입니다.");
            player.attack(monster);

            if (!monster.isAlive()) {
                System.out.println(monster.getInfo() + "를 처치했습니다!");
                player.incrementKills();
                int expGained = monster.getExpReward();
                System.out.println("경험치 " + expGained + "를 획득했습니다.");
                player.gainExperience(expGained);
                return true;
            }

            System.out.println("\n몬스터의 턴입니다...");
            monster.attack(player);

            if (!player.isAlive()) {
                System.out.println("당신은 " + monster.getInfo() + "에게 패배했습니다...");
                return false;
            }
        }

        System.out.println("===== 전투 종료 =====");
        return player.isAlive();
    }

    private void equipItemFromInventory() {
        ArrayList<Item> inventory = player.getInventory();

        if (inventory.isEmpty()) {
            return;
        }

        System.out.print("\n장착할 아이템 번호를 입력하세요 (0: 취소): ");
        int choice = getPlayerChoice(0, inventory.size());

        if (choice > 0) {
            player.equipItem(inventory.get(choice - 1));
        }
    }

    private void gameOver() {
        System.out.println("\n===== 게임 오버 =====");
        System.out.println("플레이어: " + player.getName());
        System.out.println("최종 레벨: " + player.getLevel());
        System.out.println("처치한 몬스터 수: " + player.getTotalKills());
        System.out.println("===== 게임이 종료되었습니다 =====");
    }

    private void gameCleared() {
        System.out.println("\n============================================");
        System.out.println("=           게임 클리어!                  =");
        System.out.println("============================================");
        System.out.println("축하합니다! " + player.getName() + "님은 30층의 최종 보스를 물리쳤습니다!");
        System.out.println("\n---------- 최종 플레이어 정보 ----------");
        System.out.println("이름: " + player.getName());
        System.out.println("최종 레벨: " + player.getLevel());
        System.out.println("최대 체력: " + player.getMaxHealthPoints());
        System.out.println("처치한 몬스터 수: " + player.getTotalKills());
        System.out.println("최종 무기: " + (player.getEquippedWeapon() != null ? player.getEquippedWeapon().getDescription() : "없음"));
        System.out.println("최종 방어구: " + (player.getEquippedArmor() != null ? player.getEquippedArmor().getDescription() : "없음"));

        System.out.println("\n---------- 인벤토리 ----------");
        ArrayList<Item> inventory = player.getInventory();
        if (inventory.isEmpty()) {
            System.out.println("인벤토리가 비어있습니다.");
        } else {
            for (int i = 0; i < inventory.size(); i++) {
                System.out.println(" - " + inventory.get(i).getDescription());
            }
        }

        System.out.println("============================================");
        System.out.println("=           THE END                      =");
        System.out.println("============================================");
    }

    private int getPlayerChoice(int min, int max) {
        int choice = min - 1;

        while (choice < min || choice > max) {
            try {
                choice = Integer.parseInt(scanner.next());

                if (choice < min || choice > max) {
                    System.out.print("유효한 숫자를 입력하세요 (" + min + "-" + max + "): ");
                }
            } catch (NumberFormatException e) {
                System.out.print("숫자를 입력하세요: ");
                scanner.next();
            }
        }

        return choice;
    }

    public Player getPlayer() {
        return player;
    }

    public void setPlayer(Player player) {
        this.player = player;
    }

    public Dungeon getDungeon() {
        return dungeon;
    }

    public void setDungeon(Dungeon dungeon) {
        this.dungeon = dungeon;
    }

    public Scanner getScanner() {
        return scanner;
    }
}