import javax.swing.*;
import java.awt.*;

public class TaskItemView {
    private JPanel panel;
    private Task task;

    public TaskItemView(Task task, Runnable onUpdate, Runnable onDelete) {
        this.task = task;

        panel = new JPanel();
        panel.setLayout(new BorderLayout());
        panel.setMaximumSize(new Dimension(Integer.MAX_VALUE, 50));
        panel.setPreferredSize(new Dimension(580, 50));


        JPanel checkbox = new JPanel();
        JCheckBox doneBox = new JCheckBox(task.getText());
        JPanel buttons = new JPanel();
        JButton priorityButton = new JButton(task.getPriority());
        JButton deleteButton = new JButton("Delete");

        checkbox.add(doneBox);
        panel.add(checkbox, BorderLayout.WEST);
        buttons.add(priorityButton);
        buttons.add(deleteButton);
        panel.add(buttons, BorderLayout.EAST);

        if (task.isDone()) {
            doneBox.setSelected(true);
        } else {
            doneBox.setSelected(false);
        }

        doneBox.addActionListener(e -> {
            if (doneBox.isSelected()) {
                task.setDone(true);
            } else {
                task.setDone(false);
            }
            EventQueue.invokeLater(onUpdate);
        });

        priorityButton.addActionListener(e -> {
            task.togglePriority();
            priorityButton.setText(task.getPriority());
            EventQueue.invokeLater(onUpdate);
        });

        deleteButton.addActionListener(e -> {
            EventQueue.invokeLater(onDelete);
        });



    }

    public JPanel getPanel() {
        return panel;
    }
}
