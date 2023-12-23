#include "DirectoryTable.h"

#include <QtGui/QKeyEvent>

DirectoryTable::~DirectoryTable() = default;

void DirectoryTable::keyPressEvent( QKeyEvent* event )
{
  switch( event->key() )
  {
  case Qt::Key_Home:
  case Qt::Key_End:
  case Qt::Key_PageUp:
  case Qt::Key_PageDown:
  case Qt::Key_Up:
  case Qt::Key_Down: QTableView::keyPressEvent( event ); break;
  default: emit keyPressed( event->keyCombination() ); event->ignore();
  }
}
