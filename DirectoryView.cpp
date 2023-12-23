#include "DirectoryView.h"

#include "InfoCache.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QProgressDialog>

DirectoryView::DirectoryView( InfoCache* cache, QWidget* parent )
    : QWidget{ parent },
      pathLabel{ new QLabel{ this } },
      tableView{ new DirectoryTable{ this } },
      proxyModel{ new QSortFilterProxyModel{ this } },
      model{ cache }
{
  proxyModel->setSourceModel( &model );
  proxyModel->setSortRole( DirectoryModel::SortingRole );
  tableView->setModel( proxyModel );
  tableView->verticalHeader()->hide();
  tableView->setSortingEnabled( true );
  tableView->setSelectionBehavior( QAbstractItemView::SelectionBehavior::SelectRows );
  auto header = tableView->horizontalHeader();
  if( header )
  {
    header->setSectionResizeMode( 0, QHeaderView::ResizeMode::Stretch );
  }
  else
  {
    qWarning() << "no header!?!";
  }

  auto layout = new QVBoxLayout{ this };
  layout->addWidget( pathLabel );
  layout->addWidget( tableView );

  connect( tableView, &DirectoryTable::keyPressed, this, &DirectoryView::keyPressed );
  connect( tableView, &QAbstractItemView::activated, this, &DirectoryView::activateIndex );
}

void DirectoryView::setPath( const QString& path )
{
  pathLabel->setText( path );
  model.setPath( path );
}

void DirectoryView::cdIntoCurrent()
{
  // return, enter, right-key
  cdIntoRow( getCurrentModelRow() );
}

void DirectoryView::cdIntoParent()
{
  QDir dir{ getModel().getPath() };
  dir.cdUp();
  const auto newPath = dir.absolutePath();
  setPath( newPath );
  const auto lastFile = lastSelectedFile.value( newPath );
  if( !lastFile.isEmpty() )
  {
    qInfo() << "found last file" << lastFile << "for" << newPath;
    setCurrentItem( lastFile );
    lastSelectedFile.remove( newPath );
  }
}

void DirectoryView::setCurrentItem( const QString& filename )
{
  const auto modelIndex = model.findIndex( filename );
  if( modelIndex.isValid() )
  {
    const auto tableIndex = proxyModel->mapFromSource( modelIndex );
    tableView->selectRow( tableIndex.row() );
    tableView->scrollTo( tableIndex );
    qInfo() << "setCurrentItem" << filename << tableIndex.row();
  }
}

void DirectoryView::setTableRow( int row )
{
  qInfo() << "setTableRow: selectRow" << row;
  tableView->selectRow( row );
  const auto index = proxyModel->index( row, 0 );
  qInfo() << "setTableRow: scrollTo" << index.row();
  tableView->scrollTo( index );
}

void DirectoryView::reloadModel()
{
  model.setPath( model.getPath() );
}

void DirectoryView::estimateSizes( bool forAll )
{
  QProgressDialog progress{ "Estimate directory sizes...", "Cancel", 0, 100, this };
  progress.setWindowModality( Qt::WindowModal );

  if( forAll )
  {
    model.getCache()->estimateTotalSize( model.getPath(), &progress );
    model.dataUpdated( -1 );
  }
  else
  {
    const auto row = getCurrentModelRow();
    const auto* item = model.getItem( row );
    if( !item || item->isSymLink() || !item->isDir() )
    {
      return;
    }
    model.getCache()->estimateTotalSize( item->absoluteFilePath(), &progress );
    model.dataUpdated( row );
  }

  progress.setValue( progress.maximum() );
}

int DirectoryView::getCurrentModelRow() const
{
  return getModelRow( tableView->currentIndex() );
}

const QFileInfo* DirectoryView::getCurrentFileInfo() const
{
  return model.getItem( getCurrentModelRow() );
}

int DirectoryView::getModelRow( const QModelIndex& tableIndex ) const
{
  return proxyModel->mapToSource( tableIndex ).row();
}

void DirectoryView::activateIndex( const QModelIndex& tableIndex )
{
  cdIntoRow( getModelRow( tableIndex ) );
}

void DirectoryView::cdIntoRow( int row )
{
  const auto* item = model.getItem( row );
  if( item && item->isDir() )
  {
    // remember last selected file in this directory
    lastSelectedFile.insert( model.getPath(), item->fileName() );
    setPath( item->absoluteFilePath() );
  }
}
