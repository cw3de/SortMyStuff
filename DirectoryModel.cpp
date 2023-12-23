#include "DirectoryModel.h"

#include "InfoCache.h"

#include <QtCore/QDir>

DirectoryModel::DirectoryModel( InfoCache* theCache, QObject* parent )
    : QAbstractItemModel{ parent }, cache{ theCache }
{
}

void DirectoryModel::setPath( const QString& newPath )
{
  beginResetModel();
  path = newPath;
  items = QDir{ path }.entryInfoList( QDir::Files | QDir::Dirs );
  endResetModel();
}

void DirectoryModel::dataUpdated( int row )
{
  if( row < 0 || row >= items.count() )
  {
    const auto last = items.size() - 1;
    emit dataChanged( createIndex( 0, 1, &items.at( 0 ) ),
                      createIndex( last, 1, &items.at( last ) ) );
  }
  else
  {
    const auto& item = items.at( row );
    emit dataChanged( createIndex( row, 1, &item ), createIndex( row, 2, &item ) );
  }
}

QVariant DirectoryModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole )
  {
    switch( section )
    {
    case 0: return QVariant{ "Name" };
    case 1: return QVariant{ "Size" };
    case 2: return QVariant{ "Info" };
    }
  }
  return {};
}

QModelIndex DirectoryModel::index( int row, int column, const QModelIndex& parent ) const
{
  if( row < 0 || row >= items.count() )
  {
    return {};
  }
  return createIndex( row, column, &items.at( row ) );
}

QModelIndex DirectoryModel::parent( const QModelIndex& index ) const
{
  return {};
}

int DirectoryModel::rowCount( const QModelIndex& parent ) const
{
  if( parent.isValid() )
    return 0;

  return items.count();
}

int DirectoryModel::columnCount( const QModelIndex& parent ) const
{
  if( parent.isValid() )
    return 0;

  return 3;
}

QVariant number2string( qint64 size )
{
  if( size < 0 )
    return QVariant{};
  if( size < 1024 )
    return QVariant{ QString::number( size ) };
  if( size < 1024 * 1024 )
    return QVariant{ QString::number( size / 1024.0, 'f', 2 ) + " KB" };
  if( size < 1024 * 1024 * 1024 )
    return QVariant{ QString::number( size / 1024.0 / 1024.0, 'f', 2 ) + " MB" };
  return QVariant{ QString::number( size / 1024.0 / 1024.0 / 1024.0, 'f', 2 ) + " GB" };
}

QVariant DirectoryModel::data( const QModelIndex& index, int role ) const
{
  if( !index.isValid() )
    return {};

  const auto row = index.row();
  if( row < 0 || row >= items.count() )
  {
    return {};
  }

  if( role == SortingRole ) // sorting
  {
    const auto& item = items.at( row );
    switch( index.column() )
    {
    case 0: return item.fileName();
    case 1:
      return item.isSymbolicLink() ? 0
             : item.isDir()        ? cache->getCachedSize( item.absoluteFilePath() )
                                   : item.size();
    case 2: return item.isDir() ? 0 : 1;
    }
  }
  else if( role == Qt::DisplayRole )
  {
    const auto& item = items.at( row );
    if( item.isSymbolicLink() )
    {
      switch( index.column() )
      {
      case 0: return item.fileName();
      case 1: return {};
      case 2: return QString{ "-> %1" }.arg( item.symLinkTarget() );
      }
    }
    else if( item.isDir() )
    {
      switch( index.column() )
      {
      case 0: return item.fileName();
      case 1: return number2string( cache->getCachedSize( item.absoluteFilePath() ) );
      case 2: return "Dir";
      }
    }
    else
    {
      switch( index.column() )
      {
      case 0: return item.fileName();
      case 1: return number2string( item.size() );
      case 2: return {};
      }
    }
  }
  else if( role == Qt::TextAlignmentRole )
  {
    if( index.column() == 1 )
      return QVariant{ Qt::AlignRight | Qt::AlignVCenter };
  }

  return {};
}
const QFileInfo* DirectoryModel::getItem( int row ) const
{
  if( row < 0 || row >= items.count() )
    return nullptr;

  return &items.at( row );
}

QModelIndex DirectoryModel::findIndex( const QString& name ) const
{
  for( int row = 0; row < items.count(); ++row )
  {
    if( items.at( row ).fileName() == name )
      return createIndex( row, 0, &items.at( row ) );
  }
  return {};
}
