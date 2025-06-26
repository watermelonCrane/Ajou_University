import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionListener;
import java.util.ArrayList;
import java.util.List;

public class TodoPanel extends JPanel {
    private JTextField inputField;
    private JButton addButton;
    private JPanel listPanel;
    private JScrollPane scrollPane;
    private JLabel statusLabel;
    private JComboBox<String> statusFilter;
    private JComboBox<String> priorityFilter;
    private TodoController controller;

    public TodoPanel() {
        inputField = new JTextField(25);
        addButton = new JButton("Add");

        add(inputField);
        add(addButton);

        listPanel = new JPanel();
        listPanel.setLayout(new BoxLayout(listPanel, BoxLayout.Y_AXIS));
        listPanel.setAlignmentX(Component.LEFT_ALIGNMENT);
        scrollPane = new JScrollPane(listPanel);
        scrollPane.setPreferredSize(new Dimension(600, 280));
        add(scrollPane);


        String[] statuses = {"None", "Done", "Not Done"};
        String[] priorities = {"None", "High", "Low"};
        statusFilter = new JComboBox<>(statuses);
        priorityFilter = new JComboBox<>(priorities);

        JLabel statLabel = new JLabel("Status: ");
        add(statLabel);
        add(statusFilter);

        JLabel priorityLabel = new JLabel("Priority: ");
        add(priorityLabel);
        add(priorityFilter);

        statusLabel = new JLabel();
        add(statusLabel);

        controller = new TodoController();

        refreshList();


        // Action Listnerssss
        ActionListener addAction = (e) -> {
            String text = inputField.getText();
            if (text.isEmpty()) {
                JOptionPane.showMessageDialog(null, "textfield is empty!");
            } else {
                if (!controller.addTask(inputField.getText())) JOptionPane.showMessageDialog(null, "Task already exists.");;
                refreshList();

                inputField.setText("");
            }
        };

        addButton.addActionListener(addAction);
        inputField.addActionListener(addAction);

        statusFilter.addActionListener(e -> {
            //System.out.println(statusFilter.getSelectedItem());
            refreshList();
        });

        priorityFilter.addActionListener(e->{
            refreshList();
        });

    }

    private void refreshList() {
        String statusLabelText = "            Total: " + controller.getTotalCount() + " / " + "Done: " + controller.getDoneCount() + " / " + "Not Done: " + (controller.getTotalCount() - controller.getDoneCount());
        statusLabel.setText(statusLabelText);

        listPanel.removeAll();
        listPanel.revalidate();
        listPanel.repaint();

        List<Task> taskList = controller.getFilteredTasks(statusFilter.getSelectedItem().toString(), priorityFilter.getSelectedItem().toString());
        for (var task : taskList) {
            TaskItemView newTaskView = new TaskItemView(task, ()->{
                refreshList();
            }, ()->{
                controller.removeTask(task);
                refreshList();
            });
            listPanel.add(newTaskView.getPanel());
        }

    }


    public static void main(String[] args) {
        EventQueue.invokeLater(() -> {
            JFrame frame = new JFrame("To-Do List");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setSize(600, 400);
            frame.setLocationRelativeTo(null);
            frame.add(new TodoPanel());
            frame.setVisible(true);
        });
    }
}