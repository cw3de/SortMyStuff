#pragma once

#include <QtCore/QHash>
#include <QtCore/QString>

class QProgressDialog;

class InfoCache
{
public:
  static constexpr qint64 unknown = -1;

  InfoCache() = default;

  qint64 getCachedSize( const QString& path ) const { return cache.value( path, unknown ); }
  qint64 estimateTotalSize( const QString& path, QProgressDialog* progress = nullptr );

private:
  QHash<QString, qint64> cache;
};
