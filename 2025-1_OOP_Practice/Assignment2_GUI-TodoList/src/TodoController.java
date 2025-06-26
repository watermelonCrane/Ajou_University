import java.util.ArrayList;
import java.util.List;
import java.util.stream.*;

public class TodoController {
    private List<Task> taskList = new ArrayList<>();

    public boolean addTask(String text) {
        for (var task : taskList) {
            if (task.getText().equals(text)) return false;
        }
        taskList.add(new Task(text));

        return true;
    }

    public void removeTask(Task task) {
        taskList.removeIf(t -> t.equals(task));
    }

    public List<Task> getFilteredTasks(String status, String priority) {
        Stream<Task> taskStream = taskList.stream();
        switch (status) {
            case "Done":
                taskStream = taskStream.filter(t -> t.isDone());
                break;
            case "Not Done":
                taskStream = taskStream.filter(t -> !t.isDone());
                break;
            default:
                break;
        }
        switch (priority) {
            case "High":
                taskStream = taskStream.filter(t -> t.getPriority().equals(Task.HIGH));
                break;
            case "Low":
                taskStream = taskStream.filter(t -> t.getPriority().equals(Task.LOW));
                break;
            default:
                break;
        }

        return taskStream.collect(Collectors.toList());

    }

    public int getTotalCount() {
        return taskList.size();
    }

    public int getDoneCount() {
        return (int)taskList.stream()
                .filter(Task::isDone)
                .count();
    }
}
