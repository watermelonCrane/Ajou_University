// lab13
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class SimpleCalculator extends JFrame implements ActionListener {
    private JTextField firstNumberField;
    private JTextField secondNumberField;
    private JLabel resultLabel;
    private JButton addButton, subtractButton, multiplyButton;
    private JButton divideButton, clearButton;

    class InputPanel extends JPanel {
        public InputPanel(String name, String field) {
            super(new FlowLayout(FlowLayout.LEFT));

            add(new JLabel(name));
            if (field.equals("first")) {
                firstNumberField = new JTextField(10);
                add(firstNumberField);
            }
            else if (field.equals("second")) {
                secondNumberField = new JTextField(10);
                add(secondNumberField);
            }
            setBorder(BorderFactory.createEmptyBorder(0, 40, 0, 0));    //ChatGPT를 이용해 작성한 코드, 1줄

        }
    }
    class ButtonPanel extends JPanel {
        public ButtonPanel() {
            addButton = new JButton("+");
            subtractButton = new JButton("-");
            multiplyButton = new JButton("×");
            divideButton = new JButton("÷");
            clearButton = new JButton("초기화");

            add(addButton);
            add(subtractButton);
            add(multiplyButton);
            add(divideButton);
            add(clearButton);

        }
    }
    class ResultPanel extends JPanel {
        public ResultPanel() {
            super(new FlowLayout(FlowLayout.RIGHT));
            resultLabel = new JLabel("결과 ");
            resultLabel.setForeground(Color.blue);
            add(resultLabel);

            setBorder(BorderFactory.createEmptyBorder(0, 0, 0, 40));    //ChatGPT를 이용해 작성한 코드, 1줄

        }
    }

    public SimpleCalculator() {
        setTitle("간단한 계산기");
        setSize(350, 200);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setVisible(true);
        setLocationRelativeTo(null);
        setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));       // chatGPT를 통해 작성한 부분, 1줄

        InputPanel input1 = new InputPanel("첫 번째 수: ", "first");
        add(input1);
        InputPanel input2 = new InputPanel("두 번째 수: ", "second");
        add(input2);


        ButtonPanel buttons = new ButtonPanel();
        add(buttons);

        addButton.addActionListener(this);
        subtractButton.addActionListener(this);
        multiplyButton.addActionListener(this);
        divideButton.addActionListener(this);
        clearButton.addActionListener(this);


        ResultPanel results = new ResultPanel();
        add(results);


    }

    public void actionPerformed(ActionEvent e) {
        double result = 0;
        try {
            double first = Double.parseDouble(firstNumberField.getText());
            double second = Double.parseDouble((secondNumberField.getText()));

            switch (e.getActionCommand()) {
                case "+":
                    result = first + second;
                    break;
                case "-":
                    result = first - second;
                    break;
                case "×":
                    result = first * second;
                    break;
                case "÷":
                    if (second == 0.0) throw new ArithmeticException();
                    result = first / second;
                    break;
                case "초기화":
                    firstNumberField.setText("");
                    secondNumberField.setText("");
                    resultLabel.setText("결과 ");
                    return;
            }
        }
        catch (NumberFormatException exception) {
            JOptionPane.showMessageDialog(null, "숫자만 입력할 수 있습니다.", "입력 오류", JOptionPane.ERROR_MESSAGE);
            return;
        }
        catch (ArithmeticException exception) {

            JOptionPane.showMessageDialog(null, "0으로 나눌 수 없습니다.", "입력 오류", JOptionPane.ERROR_MESSAGE);
            return;
        }

        resultLabel.setText("결과 " + firstNumberField.getText() + " " + e.getActionCommand() + " " + secondNumberField.getText() + " = " + Double.toString(result) );

        System.out.println(result);
    }

    public static void main(String[] args) {
        EventQueue.invokeLater(() -> {
            JFrame frame = new SimpleCalculator();
        });
    }
}