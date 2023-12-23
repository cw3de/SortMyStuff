#pragma once

#include <QtCore/QString>

enum class SmsType
{
  Unknown,
  Directory,
  Other, // symlinks, special files, etc
  Image,
  Audio,
  Video,
  Archive,
  Document,
  Sourcecode,
  Text,
  Temporary,
};

const char* nameOf( SmsType );
SmsType typeOf( const QString& filename );
