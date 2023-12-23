#include "InfoCache.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtWidgets/QProgressDialog>

qint64 InfoCache::estimateTotalSize( const QString& path, QProgressDialog* progress )
{
  qint64 totalSize = 0;
  QDir dir( path );
  QFileInfoList list = dir.entryInfoList( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );
  for( const QFileInfo& info : list )
  {
    if( progress && progress->wasCanceled() )
      break;

    if( info.isSymbolicLink() )
      continue;

    if( info.isDir() )
    {
      if( progress )
      {
        progress->setMaximum( progress->maximum() + 1 );
      }
      totalSize += estimateTotalSize( info.absoluteFilePath() );
    }
    else
    {
      totalSize += info.size();
    }
  }
  if( progress )
  {
    progress->setValue( progress->value() + 1 );
  }

  cache.insert( path, totalSize );
  return totalSize;
}
