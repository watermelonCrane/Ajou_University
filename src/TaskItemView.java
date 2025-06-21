import javax.swing.*;
import java.awt.*;

public class TaskItemView {
    private JPanel panel;
    private Task task;

    public TaskItemView(Task task, Runnable onUpdate, Runnable onDelete) {
        this.task = task;

        EventQueue.invokeLater(() -> {
            panel = new JPanel();

            JCheckBox doneBox = new JCheckBox(task.getText());
            JButton priorityButton = new JButton(task.getPriority());
            JButton deleteButton = new JButton("Delete");

            panel.add(doneBox);
            panel.add(priorityButton);
            panel.add(deleteButton);

            doneBox.addActionListener(e -> {
                if (doneBox.isSelected()) {
                    task.setDone(true);
                } else {
                    task.setDone(false);
                }
            });

            priorityButton.addActionListener(e -> {
                task.togglePriority();
                priorityButton.setText(task.getPriority());
                EventQueue.invokeLater(onUpdate);
            });

            deleteButton.addActionListener(e -> {
                EventQueue.invokeLater(onDelete);
            });
        });



    }

    public JPanel getPanel() {
        return panel;
    }
}
