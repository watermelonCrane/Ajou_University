/*
* TODO:
*  현재까지 완료된 부분: Task.java 완료.
*  refresh list 만드는 중. 여기서 Task들을 TaskItemView들로 감쌀것임.
*  근데 Runable들은 도대체 뭘 넣어야 하냐...
*
*
* */



import javax.swing.*;
import java.awt.*;
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
        listPanel.setPreferredSize(new Dimension(600, 280));
        scrollPane = new JScrollPane(listPanel);
        add(listPanel);
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


        addButton.addActionListener(e ->{
            controller.addTask(inputField.getText());
            refreshList();
        });

    }

    private void refreshList() {
        String statusLabelText = "            Total: " + controller.getTotalCount() + " / " + "Done: " + controller.getDoneCount() + " / " + "Not Done: " + (controller.getTotalCount() - controller.getDoneCount());
        statusLabel.setText(statusLabelText);

    }


    public static void main(String[] args) {
        EventQueue.invokeLater(() -> {
            JFrame frame = new JFrame("To-Do List");
            frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            frame.setSize(600, 400);
            frame.add(new TodoPanel());
            frame.setVisible(true);
        });
    }
}