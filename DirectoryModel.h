#pragma once
#include "SmsType.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QFileInfo>

class InfoCache;
class DirectoryModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  static constexpr int SortingRole = Qt::UserRole + 1;

  explicit DirectoryModel( InfoCache*, QObject* parent = nullptr );
  void setPath( const QString& );
  void dataUpdated( int row = -1 );

  InfoCache* getCache() const { return cache; }
  const QString& getPath() const { return path; }
  const QList<QFileInfo>& getItems() const { return items; }
  const QFileInfo* getItem( int row ) const;
  QModelIndex findIndex( const QString& name ) const;

  int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
  int columnCount( const QModelIndex& parent = QModelIndex() ) const override;
  QModelIndex index( int, int, const QModelIndex& parent = QModelIndex() ) const override;
  QModelIndex parent( const QModelIndex& ) const override;
  QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
  QVariant headerData( int, Qt::Orientation, int role = Qt::DisplayRole ) const override;

private:
  InfoCache* cache{ nullptr };
  QString path;
  QList<QFileInfo> items;
};
