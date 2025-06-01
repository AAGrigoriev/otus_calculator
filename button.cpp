#include "button.hpp"

#include <QtMath>

CalcButton::CalcButton(const QString &text, QWidget *parent)
    : QPushButton(parent) {
  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
  setText(text);
}

QSize CalcButton::sizeHint() const {
  QSize size = QPushButton::sizeHint();
  size.rheight() += 20;
  size.rwidth() = qMax(size.width(), size.height());

  return size;
}
