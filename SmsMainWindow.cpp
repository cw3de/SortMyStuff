#include "SmsMainWindow.h"

#include <QtGui/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QStatusBar>

SmsMainWindow::SmsMainWindow()
    : QMainWindow{},
      treeView{ new QTreeView{ this } },
      proxyModel{ new QSortFilterProxyModel{ this } }
{
  toolBar = addToolBar( "Main" );

  auto scanAction = new QAction( "&Scan", this );
  toolBar->addAction( scanAction );
  connect( scanAction, &QAction::triggered, this, &SmsMainWindow::chooseDir );

  auto cancelAction = new QAction( "&Cancel", this );
  toolBar->addAction( cancelAction );
  connect( cancelAction, &QAction::triggered, this, &SmsMainWindow::cancelScan );

  proxyModel->setSourceModel( &model );
  treeView->setModel( proxyModel );
  treeView->setSortingEnabled( true );
  setCentralWidget( treeView );

  connect( &model, &SmsModel::scanStarted, this, &SmsMainWindow::onScanStarted );
  connect( &model, &SmsModel::scanProgress, this, &SmsMainWindow::onScanProgress );
  connect( &model, &SmsModel::scanCanceled, this, &SmsMainWindow::onScanCanceled );
  connect( &model, &SmsModel::scanFinished, this, &SmsMainWindow::onScanFinished );
}

SmsMainWindow::~SmsMainWindow() = default;

void SmsMainWindow::chooseDir()
{
  const auto path = QFileDialog::getExistingDirectory( this, tr( "Open Directory" ) );
  if( !path.isEmpty() )
  {
    model.startScanner( path );
  }
}

void SmsMainWindow::cancelScan()
{
  model.cancelScanner();
}

void SmsMainWindow::onScanStarted()
{
  statusBar()->showMessage( "Scanning..." );
}

void SmsMainWindow::onScanProgress( int dirsTodo, int dirsDone, const QString& )
{
  statusBar()->showMessage( QString( "Scanning... %1/%2" ).arg( dirsTodo ).arg( dirsDone ) );
}

void SmsMainWindow::onScanCanceled()
{
  statusBar()->showMessage( "Scan canceled" );
}

void SmsMainWindow::onScanFinished()
{
  statusBar()->showMessage( "Scan finished" );
}
