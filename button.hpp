#pragma once

#include <QPushButton>

//! [0]
class CalcButton : public QPushButton {
  Q_OBJECT

 public:
  explicit CalcButton(const QString &text, QWidget *parent = nullptr);

  QSize sizeHint() const override;
};
