#pragma once

#include <QtCore/QString>

#include <exception>

class FileMover
{
public:
  class Error : public std::exception
  {
  public:
    Error( const QString& msg ) : message{ msg.toUtf8() } {}
    const char* what() const noexcept override { return message.constData(); }

  private:
    QByteArray message;
  };

  FileMover() = delete;
  static bool move( const QString& srcDir,
                    const QString& dstDir,
                    const QString& filename,
                    bool renameDuplicates );
};
