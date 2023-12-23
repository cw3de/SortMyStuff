#include "FileMover.h"

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>

static QString getUniqueFilename( const QDir& dir, const QString& filename )
{
  int i = 0;
  QString newName = filename;
  while( QFile::exists( dir.absoluteFilePath( newName ) ) )
  {
    newName = QString{ "%1_%2" }.arg( filename ).arg( ++i );
  }
  return dir.absoluteFilePath( newName );
}

static bool isSameDevice( const QString& pathname1, const QString& pathname2 )
{
  // TODO: copy/delete on cross-device move
  return true;
}

static void renameOrThrow( const QString& srcPathname, const QString& dstPathname )
{
  if( !QFile::rename( srcPathname, dstPathname ) )
  {
    throw FileMover::Error{
      QString{ "Failed to rename %1 to %2" }.arg( srcPathname ).arg( dstPathname )
    };
  }
}

static void moveOneFile( const QString& srcPathname, const QString& dstPathname )
{
  if( isSameDevice( srcPathname, dstPathname ) )
  {
    renameOrThrow( srcPathname, dstPathname );
  }
  else
  {
    if( !QFile::copy( srcPathname, dstPathname ) )
    {
      throw FileMover::Error{
        QString{ "Failed to copy %1 to %2" }.arg( srcPathname ).arg( dstPathname )
      };
    }
    if( !QFile::remove( srcPathname ) )
    {
      throw FileMover::Error{ QString{ "Failed to remove %1" }.arg( srcPathname ) };
    }
  }
}

static int moveFiles( const QString& srcDirname,
                      const QString& dstDirname,
                      const QString& onlyFilename,
                      bool renameDuplicates )
{
  QDir srcDir{ srcDirname };
  if( !srcDir.exists() )
  {
    throw FileMover::Error{ "Source directory does not exist" };
  }

  QDir dstDir{ dstDirname };
  if( !dstDir.exists() )
  {
    throw FileMover::Error{ "Destination directory does not exist" };
  }

  int filesLeft{ 0 };
  const auto filter = QDir::Filter::Dirs | QDir::Filter::Files | QDir::Filter::NoDotAndDotDot;

  const auto entries = onlyFilename.isEmpty()
                           ? srcDir.entryInfoList( filter )
                           : srcDir.entryInfoList( QStringList{ onlyFilename }, filter );

  qInfo() << "move" << entries.size() << "entries from" << srcDirname << "to" << dstDirname;
  for( const auto& srcInfo : entries )
  {
    const auto filename = srcInfo.fileName();
    const auto srcPath = srcDir.absoluteFilePath( filename );
    const auto dstPath = dstDir.absoluteFilePath( filename );
    const QFileInfo dstInfo{ dstPath };

    if( dstInfo.exists() )
    {
      if( srcInfo.isFile() == dstInfo.isFile() )
      {
        if( renameDuplicates )
        {
          const auto newPathname = getUniqueFilename( dstDir, filename );

          if( srcInfo.lastModified() > dstInfo.lastModified() )
          {
            qInfo() << srcPath << "is newer than" << dstPath << "->" << newPathname;
            renameOrThrow( dstPath, newPathname );
            moveOneFile( srcPath, dstPath );
          }
          else
          {
            qInfo() << srcPath << "is older than" << dstPath << "->" << newPathname;
            moveOneFile( srcPath, newPathname );
          }
        }
        else
        {
          qInfo() << dstPath << "already exists";
          ++filesLeft;
        }
      }
      else if( srcInfo.isDir() && dstInfo.isDir() )
      {
        const auto subFilesLeft = moveFiles( srcPath, dstPath, {}, renameDuplicates );
        if( !subFilesLeft )
        {
          qInfo() << "moved" << srcPath << "to" << dstPath << ", no files left";
          if( !srcDir.rmdir( srcPath ) )
          {
            throw FileMover::Error{ "Failed to remove directory" };
          }
        }
        else
        {
          qInfo() << "moved" << srcPath << "to" << dstPath << "," << subFilesLeft
                  << "files left";
          filesLeft += subFilesLeft;
        }
      }
      else
      {
        qWarning() << srcPath << "is" << ( srcInfo.isFile() ? "file" : "dir" ) << ", but"
                   << dstPath << "is" << ( dstInfo.isFile() ? "file" : "dir" );
        ++filesLeft;
      }
    }
    else
    {
      qInfo() << "move" << srcPath << "to non existing" << dstPath;
      if( srcInfo.isFile() )
      {
        qInfo() << "move file" << srcPath << "to" << dstPath;
        moveOneFile( srcPath, dstPath );
      }
      else if( srcInfo.isDir() )
      {
        if( isSameDevice( srcPath, dstPath ) )
        {
          qInfo() << "move dir" << srcPath << "to" << dstPath;
          renameOrThrow( srcPath, dstPath );
        }
        else
        {
          qInfo() << "copy/delete dir" << srcPath << "to" << dstPath;
          if( !dstDir.mkdir( filename ) )
          {
            throw FileMover::Error{ "Failed to create directory" };
          }
          if( moveFiles( srcPath, dstPath, {}, false ) != 0 )
          {
            throw FileMover::Error{ "Failed to move directory" };
          }
          srcDir.rmdir( filename );
        }
      }
    }
  }

  return filesLeft;
}

bool FileMover::move( const QString& srcDirname,
                      const QString& dstDirname,
                      const QString& filename,
                      bool renameDuplicates )
{
  return moveFiles( srcDirname, dstDirname, filename, renameDuplicates ) == 0;
}
