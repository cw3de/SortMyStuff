#include "DualPaneView.h"

#include "FileMover.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QBoxLayout>
#include <QtWidgets/QMessageBox>

DualPaneView::DualPaneView()
    : leftView{ new DirectoryView{ &cache, this } },
      rightView{ new DirectoryView{ &cache, this } }
{
  setCentralWidget( new QWidget{ this } );
  auto layout = new QHBoxLayout{ centralWidget() };
  layout->addWidget( leftView );
  layout->addWidget( rightView );
  const auto homePath = QDir::homePath();
  leftView->setPath( homePath );
  leftView->setObjectName( "Left" );
  rightView->setPath( homePath );
  rightView->setObjectName( "Right" );

  connect( leftView, &DirectoryView::keyPressed, this, &DualPaneView::gotKeyPressed );
  connect( rightView, &DirectoryView::keyPressed, this, &DualPaneView::gotKeyPressed );
}

void DualPaneView::gotKeyPressed( QKeyCombination keyCombo )
{
  auto from = dynamic_cast<DirectoryView*>( sender() );
  if( !from )
  {
    qWarning() << "DualPaneView::gotKeyPressed: sender is not a DirectoryView!?!";
    return;
  }

  const bool shiftPressed
      = keyCombo.keyboardModifiers().testAnyFlag( Qt::KeyboardModifier::ShiftModifier );

  switch( keyCombo.key() )
  {
    // change active pane
  case Qt::Key_Tab:
    if( from == leftView )
    {
      activeView = rightView;
    }
    else
    {
      activeView = leftView;
    }
    activeView->setFocus();
    break;
    // activate current item
  case Qt::Key_Enter:
  case Qt::Key_Return:
  case Qt::Key_Right:
  case Qt::Key_L: showItem( from ); break;
  case Qt::Key_Left:
  case Qt::Key_H:
    from->cdIntoParent();
    break;
    // move from one pane to the other
  case Qt::Key_M:
    if( from == leftView )
    {
      moveCurrentItem( leftView, rightView, shiftPressed );
    }
    else if( from == rightView )
    {
      moveCurrentItem( rightView, leftView, shiftPressed );
    }
    break;
  case Qt::Key_S: updateSize( from, shiftPressed ); break;
  case Qt::Key_X: removeItem( from ); break;
  case Qt::Key_Escape: qApp->quit(); break;
  default:
    qInfo() << "DualPaneView::gotKeyPressed" << keyCombo.key() << keyCombo.keyboardModifiers()
            << from->objectName();
    break;
  }
}

void DualPaneView::keyPressEvent( QKeyEvent* event )
{
  // qDebug() << "DualPaneView::keyPressEvent" << event->text();
  QMainWindow::keyPressEvent( event );
}

void DualPaneView::updateSize( DirectoryView* view, bool all )
{
  if( !view )
  {
    qWarning() << "DualPaneView::updateSize: view is null!?!";
    return;
  }

  view->estimateSizes( all );
}

void DualPaneView::moveCurrentItem( DirectoryView* src,
                                    DirectoryView* dst,
                                    bool renameDuplicates )
{
  const auto tableRow = src->getTableRow(); // remember the current row
  const auto* current = src->getCurrentFileInfo();
  if( current )
  {
    try
    {
      FileMover::move( src->getPath(), dst->getPath(), current->fileName(), renameDuplicates );
    }
    catch( const FileMover::Error& ex )
    {
      QMessageBox::warning( this, "Failed to move file", ex.what() );
    }
    src->reloadModel();
    dst->reloadModel();
    src->setTableRow( tableRow ); // set the current row in src back to what it was
  }
  // TODO: select next item in src
}

void DualPaneView::showItem( DirectoryView* view )
{
  if( !view )
  {
    qWarning() << "DualPaneView::showItem: view is null!?!";
    return;
  }
  const auto* current = view->getCurrentFileInfo();
  if( !current )
  {
    return;
  }

  if( current->isDir() )
  {
    view->cdIntoCurrent();
  }
  else
  {
    // TODO: show contents of file
    QMessageBox::information( this, "Show Item", current->absoluteFilePath() );
  }
}

void DualPaneView::removeItem( DirectoryView* view )
{
  if( !view )
  {
    qWarning() << "DualPaneView::showItem: view is null!?!";
    return;
  }
  const auto* current = view->getCurrentFileInfo();
  if( !current )
  {
    return;
  }
  const auto tableRow = view->getTableRow();
  if( current->isSymbolicLink() || current->isFile() )
  {
    if( !QFile::remove( current->absoluteFilePath() ) )
    {
      QMessageBox::warning( this, "Failed to remove file", current->absoluteFilePath() );
    }
  }
  else
  {
    if( QMessageBox::question( this,
                               "Remove Directory",
                               QString{ "remove %1?" }.arg( current->absoluteFilePath() ),
                               QMessageBox::Yes | QMessageBox::No )
        != QMessageBox::Yes )
    {
      return;
    }

    QDir dir{ current->absoluteFilePath() };
    if( !dir.removeRecursively() )
    {
      QMessageBox::warning( this, "Failed to remove directory", current->absoluteFilePath() );
    }
  }
  view->reloadModel();
  view->setTableRow( tableRow ); // set the current row in src back to what it was
}
