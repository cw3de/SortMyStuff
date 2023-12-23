#pragma once

#include "SmsModel.h"

#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QTreeView>

class SmsMainWindow : public QMainWindow
{
  Q_OBJECT
public:
  SmsMainWindow();
  ~SmsMainWindow() override;

private slots:
  void chooseDir();
  void cancelScan();
  void onScanStarted();
  void onScanProgress( int todo, int done, const QString& );
  void onScanCanceled();
  void onScanFinished();

private:
  QToolBar* toolBar{ nullptr };
  QTreeView* treeView{ nullptr };
  QSortFilterProxyModel* proxyModel{ nullptr };
  SmsModel model;
};
