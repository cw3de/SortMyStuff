#include "SmsModel.h"

SmsModel::SmsModel( QObject* parent ) : QAbstractItemModel( parent ) {}

void SmsModel::startScanner( const QString& path )
{
  if( scanner )
    return;

  beginResetModel();
  scanner = new SmsScanner( &tree, path );
  connect( scanner, &SmsScanner::scanStarted, this, &SmsModel::scanStarted );
  connect( scanner, &SmsScanner::scanProgress, this, &SmsModel::scanProgress );
  connect( scanner, &SmsScanner::scanCanceled, this, &SmsModel::onScanCanceled );
  connect( scanner, &SmsScanner::scanFinished, this, &SmsModel::onScanFinished );
  scanner->start();
}

void SmsModel::cancelScanner()
{
  if( scanner )
  {
    scanner->requestInterruption();
  }
}

QVariant SmsModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
  if( orientation == Qt::Orientation::Horizontal && role == Qt::DisplayRole )
  {
    switch( section )
    {
    case 0: return QVariant{ "Name" };
    case 1: return QVariant{ "Size" };
    case 2: return QVariant{ "Type" };
    }
  }
  return {};
}

static SmsNode* getNodeAt( SmsNode* parent, int index )
{
  if( parent == nullptr )
    return nullptr;

  if( index < 0 || index >= parent->getChildren().count() )
  {
    // qWarning() << index << "index out of range";
    return nullptr;
  }
  auto node = parent->getChildren().at( index );
  Q_ASSERT( node );
  return node;
}

static int getIndexOf( SmsNode* node )
{
  if( !node )
    return 0;
  auto parent = node->getParent();
  if( !parent )
    return 0;
  auto index = parent->getChildren().indexOf( node );
  return index;
}

static SmsNode* getNodeFrom( const QModelIndex& modelIndex )
{
  auto node = static_cast<SmsNode*>( modelIndex.internalPointer() );
  Q_ASSERT( node );
  return node;
}

QModelIndex SmsModel::index( int row, int column, const QModelIndex& parent ) const
{
  auto parentNode = parent.isValid() ? getNodeFrom( parent ) : tree.getRoot();
  auto childNode = getNodeAt( parentNode, row );
  if( childNode )
    return createIndex( row, column, childNode );

  return {};
}

QModelIndex SmsModel::parent( const QModelIndex& index ) const
{
  if( !index.isValid() )
    return {};

  auto childItem = getNodeFrom( index );
  if( !childItem )
  {
    qWarning() << "parent() called without valid node";
    return {};
  }

  auto parentItem = childItem->getParent();
  if( !parentItem )
  {
    qWarning() << "parent() called without parent";
    return {};
  }

  if( parentItem == tree.getRoot() )
    return {};

  return createIndex( getIndexOf( parentItem ), 0, parentItem );
}

int SmsModel::rowCount( const QModelIndex& parent ) const
{
  if( parent.column() > 0 )
    return 0;

  if( !parent.isValid() )
  {
    if( tree.getRoot() )
    {
      return tree.getRoot()->getChildren().count();
    }
    return 0;
  }

  return getNodeFrom( parent )->getChildren().count();
}

int SmsModel::columnCount( const QModelIndex& parent ) const
{
  return 3; // Name, Size, Content
}

QVariant SmsModel::data( const QModelIndex& index, int role ) const
{
  if( !index.isValid() )
    return {};

  if( role == Qt::DisplayRole )
  {
    auto node = getNodeFrom( index );
    switch( index.column() )
    {
    case 0: return node->getName();
    case 1: return node->getSize();
    case 2: return node->isDir() ? node->getCounter().toString() : nameOf( node->getType() );
    }
  }
  else if( role == Qt::TextAlignmentRole )
  {
    if( index.column() == 1 )
      return Qt::AlignRight;
  }

  return QVariant();
}

void SmsModel::onScanCanceled()
{
  scanner->deleteLater();
  scanner = nullptr;
  endResetModel();
  emit scanCanceled();
}

void SmsModel::onScanFinished()
{
  scanner->deleteLater();
  scanner = nullptr;
  endResetModel();
  emit scanFinished();
}
