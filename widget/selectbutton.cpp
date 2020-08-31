#include "selectbutton.h"

#include <QIcon>
#include <QString>

SelectButton::SelectButton(const QString &text, const QIcon &icon_outline_, const QIcon &icon_, QWidget *parent) : icon_outline(icon_outline_), icon(icon_), QToolButton(parent) {
  setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
  setText(text);
  setIcon(icon_outline);
  setIconSize(QSize(32, 32));
  setCheckable(true);
  setMinimumSize(96, 78);
  connect(this, SIGNAL(toggled(bool)), this, SLOT(onClick(bool)));
}

void SelectButton::onClick(bool e) {
  if (e) {
    setIcon(icon);
  } else {
    setIcon(icon_outline);
  }
}
