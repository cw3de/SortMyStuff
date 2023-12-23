#include "SmsType.h"

const char* nameOf( SmsType t )
{
  switch( t )
  {
  case SmsType::Unknown: return "Unknown";
  case SmsType::Directory: return "Directory";
  case SmsType::Other: return "Symlink";
  case SmsType::Audio: return "Audio";
  case SmsType::Video: return "Video";
  case SmsType::Image: return "Image";
  case SmsType::Archive: return "Archive";
  case SmsType::Document: return "Document";
  case SmsType::Sourcecode: return "Source";
  case SmsType::Text: return "Text";
  case SmsType::Temporary: return "Temporary";
  }
}

static struct
{
  const char* filename;
  SmsType type;
} nameToType[] = {
  { "CMakeLists.txt", SmsType::Sourcecode }, //
  { "CMakeLists.txt.user", SmsType::Sourcecode },
  { "go.mod", SmsType::Sourcecode },
  { "go.sum", SmsType::Sourcecode },
  { "Makefile", SmsType::Sourcecode },
};

static struct
{
  const char* extension;
  SmsType type;
} extToType[] = {
  { "7z", SmsType::Archive },
  // a
  { "avi", SmsType::Video }, //
  // b
  { "bmp", SmsType::Image },
  { "bz2", SmsType::Archive },
  { "bak", SmsType::Temporary },
  // c
  { "c", SmsType::Sourcecode },
  { "cc", SmsType::Sourcecode },
  { "cpp", SmsType::Sourcecode },
  { "cs", SmsType::Sourcecode },
  { "css", SmsType::Sourcecode },
  { "csv", SmsType::Text },
  // d
  { "dmg", SmsType::Archive },
  { "doc", SmsType::Document },
  { "docx", SmsType::Document },
  // e
  // f
  { "flac", SmsType::Audio },
  // g
  { "gif", SmsType::Image },
  { "gz", SmsType::Archive },
  { "go", SmsType::Sourcecode },
  // h
  { "h", SmsType::Sourcecode },
  { "hh", SmsType::Sourcecode },
  { "hpp", SmsType::Sourcecode },
  { "html", SmsType::Sourcecode },
  // i
  { "iso", SmsType::Archive },
  // j
  { "jpg", SmsType::Image },
  { "jpeg", SmsType::Image },
  { "java", SmsType::Sourcecode },
  { "js", SmsType::Sourcecode },
  { "json", SmsType::Text },
  // k
  // l
  { "ldt", SmsType::Text },
  // m
  { "mp3", SmsType::Audio },
  { "mp4", SmsType::Video },
  { "mkv", SmsType::Video },
  { "mov", SmsType::Video },
  { "md", SmsType::Text },
  // n
  // o
  { "o", SmsType::Temporary },
  { "ods", SmsType::Document },
  { "odt", SmsType::Document },
  { "ogg", SmsType::Audio },
  { "old", SmsType::Temporary },
  // p
  { "pdf", SmsType::Document },
  { "png", SmsType::Image },
  { "py", SmsType::Sourcecode },
  { "php", SmsType::Sourcecode },
  // q
  // r
  { "rar", SmsType::Archive },
  { "rtf", SmsType::Document },
  // s
  { "sh", SmsType::Sourcecode },
  { "sql", SmsType::Text },
  { "swp", SmsType::Temporary },
  // t
  { "tar", SmsType::Archive },
  { "tcl", SmsType::Sourcecode },
  { "tex", SmsType::Text },
  { "tif", SmsType::Image },
  { "tmp", SmsType::Temporary },
  { "txt", SmsType::Text },
  // u
  { "ui", SmsType::Sourcecode },
  // v
  // w
  { "wav", SmsType::Audio },
  { "wmv", SmsType::Video },
  // x
  { "xls", SmsType::Document },
  { "xlsx", SmsType::Document },
  { "xml", SmsType::Text },
  { "xsd", SmsType::Text },
  // y
  { "yaml", SmsType::Text },
  { "yml", SmsType::Text },
  // z
  { "zip", SmsType::Archive },
};

SmsType typeOf( const QString& filename )
{
  for( const auto& ft : nameToType )
  {
    if( ft.filename == filename )
      return ft.type;
  }

  auto extPos = filename.lastIndexOf( '.' );
  if( extPos == -1 )
    return SmsType::Unknown;

  auto ext = filename.mid( extPos + 1 ).toLower();
  for( const auto& et : extToType )
  {
    if( et.extension == ext )
      return et.type;
  }

  return SmsType::Unknown;
}
