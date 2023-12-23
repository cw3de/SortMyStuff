#pragma once

#include "DirectoryModel.h"
#include "DirectoryTable.h"

#include <QtCore/QHash>
#include <QtCore/QSortFilterProxyModel>
#include <QtWidgets/QLabel>

class InfoCache;

class DirectoryView : public QWidget
{
  Q_OBJECT
public:
  DirectoryView( InfoCache*, QWidget* parent = nullptr );
  ~DirectoryView() override = default;
  void setPath( const QString& path );
  void cdIntoCurrent();
  void cdIntoParent();
  void cdIntoRow( int row );
  void reloadModel();
  void estimateSizes( bool forAll );
  void setCurrentItem( const QString& filename );
  void setTableRow( int row );

  const DirectoryModel& getModel() const { return model; }
  const QString& getPath() const { return model.getPath(); }
  int getCurrentModelRow() const;
  const QFileInfo* getCurrentFileInfo() const;
  int getModelRow( const QModelIndex& tableIndex ) const;
  int getTableRow() const { return tableView->currentIndex().row(); }

public slots:
  void activateIndex( const QModelIndex& tableIndex );

signals:
  void keyPressed( QKeyCombination );

private:
  QLabel* pathLabel{ nullptr };
  DirectoryTable* tableView{ nullptr };
  QSortFilterProxyModel* proxyModel{ nullptr };
  DirectoryModel model;
  QHash<QString, QString> lastSelectedFile;
};
