#pragma once

#include <QtWidgets/QTableView>

class DirectoryTable : public QTableView
{
  Q_OBJECT
public:
  DirectoryTable( QWidget* parent = nullptr ) : QTableView{ parent } {}
  virtual ~DirectoryTable() override;

signals:
  void keyPressed( QKeyCombination );

private:
  virtual void keyPressEvent( QKeyEvent* event ) override;
};
