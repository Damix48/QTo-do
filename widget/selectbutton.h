#ifndef SELECTBUTTON_H
#define SELECTBUTTON_H

#include <QIcon>
#include <QString>
#include <QToolButton>

#include "QListWidget"

class SelectButton : public QToolButton {
  Q_OBJECT
 private:
  QIcon icon_outline;
  QIcon icon;

 public:
  SelectButton(const QString &, const QIcon &, const QIcon &, QWidget * = nullptr);

 private slots:
  void onClick(bool);
};

#endif  // SELECTBUTTON_H
