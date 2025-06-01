#pragma once

#include <QLineEdit>
#include <QWidget>

class CalculatorWidget : public QWidget {
  Q_OBJECT

 private:
  enum class Operator { SUM, MIN, MUL, DIV, NONE };

 public:
  CalculatorWidget(QWidget* parent = nullptr);
  ~CalculatorWidget();

 private slots:
  void digitsClicked();
  void pointClicked();
  void changeSignClicked();
  void binaryOperatorClicked();
  void backspaceClicked();
  void equalClicked();
  void clear();
  void clearAll();
  void abortOperation();

 private:
  bool calcBinaryOpt(double operand);

 private:
  double calcResult{};
  double memoryResult{};
  bool waitingOperand = true;
  Operator lastOperator{Operator::NONE};
  QLineEdit* textInput;
};
