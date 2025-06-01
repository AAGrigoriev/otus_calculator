#include "calulatorwidget.hpp"

#include <QGridLayout>
#include <QLineEdit>
#include <QString>
#include <array>
#include <cmath>

#include "button.hpp"

CalculatorWidget::CalculatorWidget(QWidget* parent) : QWidget(parent) {
  auto createButton = [this](const auto text, const auto& method) {
    CalcButton* button = new CalcButton(text);
    connect(button, &CalcButton::clicked, this, method);
    return button;
  };

  textInput = new QLineEdit("0");
  textInput->setReadOnly(true);
  textInput->setAlignment(Qt::AlignRight);
  textInput->setMaxLength(20);

  QFont font = textInput->font();
  font.setPointSize(font.pointSize() + 5);
  textInput->setFont(font);

  std::array<CalcButton*, 10> buttonArray;

  for (std::size_t i{}; i < buttonArray.size(); ++i) {
    buttonArray[i] =
        createButton(QString::number(i), &CalculatorWidget::digitsClicked);
  }

  auto pointButton = createButton(tr("."), &CalculatorWidget::pointClicked);
  auto changeSignButton =
      createButton(tr("\302\261"), &CalculatorWidget::changeSignClicked);
  auto backSpaceButton =
      createButton(tr("DEL"), &CalculatorWidget::backspaceClicked);
  auto clearButton = createButton("C", &CalculatorWidget::clear);
  auto clearAllButton = createButton("AC", &CalculatorWidget::clearAll);
  auto divisionButton =
      createButton(tr("\303\267"), &CalculatorWidget::binaryOperatorClicked);
  auto multButton =
      createButton(tr("\303\227"), &CalculatorWidget::binaryOperatorClicked);
  auto minusButton =
      createButton(tr("-"), &CalculatorWidget::binaryOperatorClicked);
  auto plusButton =
      createButton(tr("+"), &CalculatorWidget::binaryOperatorClicked);
  auto equalButton = createButton(tr("="), &CalculatorWidget::equalClicked);

  auto gridLyaout = new QGridLayout;

  gridLyaout->setSizeConstraint(QLayout::SetFixedSize);
  gridLyaout->addWidget(textInput, 0, 0, 1, 4);
  gridLyaout->addWidget(backSpaceButton, 1, 0);
  gridLyaout->addWidget(clearButton, 1, 1);
  gridLyaout->addWidget(clearAllButton, 1, 2);
  gridLyaout->addWidget(equalButton, 1, 3);

  for (auto i = 1; i < buttonArray.size(); ++i) {
    int row = std::ceil(i / 3.f) + 1;
    int col = (i - 1) % 3;

    gridLyaout->addWidget(buttonArray[i], row, col);
  }

  gridLyaout->addWidget(buttonArray[0], 5, 0);
  gridLyaout->addWidget(pointButton, 5, 1);
  gridLyaout->addWidget(changeSignButton, 5, 2);

  gridLyaout->addWidget(divisionButton, 2, 3);
  gridLyaout->addWidget(multButton, 3, 3);
  gridLyaout->addWidget(minusButton, 4, 3);
  gridLyaout->addWidget(plusButton, 5, 3);
  setLayout(gridLyaout);

  setWindowTitle("OtusCalculator");
}

CalculatorWidget::~CalculatorWidget() {}

void CalculatorWidget::digitsClicked() {
  if (const auto button = qobject_cast<CalcButton*>(sender())) {
    if (const auto digitValue = button->text().toInt();
        textInput->text() != "0" || digitValue != 0) {
      if (waitingOperand) {
        textInput->clear();
        waitingOperand = false;
      }

      textInput->setText(textInput->text() + QString::number(digitValue));
    }
  }
}

void CalculatorWidget::pointClicked() {
  if (waitingOperand) {
    textInput->setText("0");
  }

  if (!textInput->text().contains('.')) {
    textInput->setText(textInput->text() + tr("."));
  }
  waitingOperand = false;
}

void CalculatorWidget::changeSignClicked() {
  auto text = textInput->text();
  const auto value = text.toDouble();

  if (value > 0.0) {
    text.prepend(tr("-"));
  } else {
    text.remove(0, 1);
  }
  textInput->setText(text);
}

void CalculatorWidget::binaryOperatorClicked() {
  if (const auto button = qobject_cast<CalcButton*>(sender())) {
    const auto operand = textInput->text().toDouble();

    if (lastOperator != Operator::NONE && !waitingOperand) {
      if (calcBinaryOpt(operand)) {
        textInput->setText(QString::number(memoryResult));
        lastOperator = Operator::NONE;
      } else {
        abortOperation();
      }
    }

    const auto buttonOperator = button->text();

    if (QString::compare(buttonOperator, "/", Qt::CaseInsensitive) == 0) {
      lastOperator = Operator::DIV;
    } else if (QString::compare(buttonOperator, "*", Qt::CaseInsensitive) ==
               0) {
      lastOperator = Operator::MUL;
    } else if (QString::compare(buttonOperator, "+", Qt::CaseInsensitive) ==
               0) {
      lastOperator = Operator::SUM;
    } else if (QString::compare(buttonOperator, "-", Qt::CaseInsensitive) ==
               0) {
      lastOperator = Operator::MIN;
    }
    memoryResult = operand;
    waitingOperand = true;
  }
}

void CalculatorWidget::equalClicked() {
  const auto operand = textInput->text().toDouble();

  if (lastOperator != Operator::NONE && !waitingOperand) {
    if (calcBinaryOpt(operand)) {
      textInput->setText(QString::number(memoryResult));
      lastOperator = Operator::NONE;
    } else {
      abortOperation();
    }
  }
  waitingOperand = true;
}

void CalculatorWidget::clear() {
  if (waitingOperand) return;

  textInput->setText("0");
  waitingOperand = true;
}

void CalculatorWidget::clearAll() {
  textInput->setText("0");
  waitingOperand = true;
  memoryResult = 0.0;
  lastOperator = Operator::NONE;
}

void CalculatorWidget::abortOperation() {
  clearAll();
  textInput->setText(tr("####"));
}

bool CalculatorWidget::calcBinaryOpt(double operand) {
  switch (lastOperator) {
    case Operator::SUM:
      memoryResult += operand;
      break;
    case Operator::DIV:
      if (operand == 0.0) {
        return false;
      }
      memoryResult /= operand;
      break;
    case Operator::MIN:
      memoryResult -= operand;
      break;
    case Operator::MUL:
      memoryResult *= operand;
      break;
    default:
      return false;
  }
  return true;
}

void CalculatorWidget::backspaceClicked() {
  if (waitingOperand) return;

  QString text = textInput->text();
  text.chop(1);
  if (text.isEmpty()) {
    text = "0";
    waitingOperand = true;
  }
  textInput->setText(text);
}
