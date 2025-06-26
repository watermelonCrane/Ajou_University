public class Task {

    private String text;
    private boolean done;
    private String priority;

    //새로 추가한 문자열 상수 필드
    final static String LOW = "LOW";
    final static String HIGH = "HIGH";

    public Task(String text) {
        this.text = text;
        this.done = false;
        this.priority = LOW;
    }

    public String getText() {
        return text;
    }

    public boolean isDone() {
        return done;
    }
    public void setDone(boolean done) {
        this.done = done;
    }

    public String getPriority() {
        return priority;
    }

    public void togglePriority() {
        if (priority == LOW) priority = HIGH;
        else priority = LOW;
    }
}
