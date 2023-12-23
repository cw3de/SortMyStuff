#include "SmsTree.h"

#include <QtCore/QDir>

SmsTree::SmsTree() {}

SmsTree::~SmsTree()
{
  clear();
}

void SmsTree::clear()
{
  qDeleteAll( allNodes );
  root = nullptr;
  allNodes.clear();
}

void SmsTree::synchronousScan( const QString& rootPath )
{
  SmsScanner scanner{ this, rootPath };
  scanner.run();
}

SmsScanner::SmsScanner( SmsTree* theTree, const QString& thePath )
    : tree{ theTree }, rootPath{ thePath }
{
}

SmsScanner::~SmsScanner() = default;

void SmsScanner::run()
{
  QDir rootDir{ rootPath };
  tree->clear();
  tree->root = new SmsNode{ rootDir.absolutePath(), rootDir.dirName(), 0, nullptr };

  QList<SmsNode*> todoList;
  todoList.append( tree->root );
  int dirsDone{ 0 };
  bool canceled{ false };
  emit scanStarted();

  while( !todoList.isEmpty() && !canceled )
  {
    const auto todoNode = todoList.takeFirst();
    emit scanProgress( todoList.size(), dirsDone, todoNode->getPath() );
    QDir dir{ todoNode->getPath() };
    const auto entries = dir.entryInfoList( QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot );

    for( const auto& dirEntry : entries )
    {
      const auto pathname = dirEntry.absoluteFilePath();

      if( dirEntry.isDir() )
      {
        auto dirNode = new SmsNode{ pathname, dirEntry.fileName(), 0, todoNode };
        tree->allNodes.append( dirNode );
        todoList.append( dirNode );
        todoNode->addChild( dirNode );
      }
      else if( dirEntry.isFile() )
      {
        auto type = typeOf( dirEntry.fileName() );
        auto fileNode = new SmsNode{ dirEntry.fileName(), dirEntry.size(), type, todoNode };
        tree->allNodes.append( fileNode );
        todoNode->addChild( fileNode );
      }
      else
      {
        auto otherNode = new SmsNode{ dirEntry.fileName(), 0, SmsType::Other, todoNode };
        tree->allNodes.append( otherNode );
        todoNode->addChild( otherNode );
      }
    }
    ++dirsDone;

    if( isInterruptionRequested() )
    {
      canceled = true;
      break;
    }
  }

  if( canceled )
  {
    emit scanCanceled();
  }
  else
  {
    emit scanFinished();
  }
}

/*
Checksum getChecksum( const QString& pathname )
{
  QFile file{ pathname };
  if( !file.open( QIODevice::ReadOnly ) )
  {
    return 0;
  }

  Checksum checksum = initChecksumValue;
  char buffer[ 4096 ];
  qint64 len;
  while( ( len = file.read( buffer, sizeof buffer ) ) > 0 )
  {
    checksum = updateFnv1a( buffer, len, checksum );
  }
  return checksum;
}
*/
