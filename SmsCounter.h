#pragma once

#include "SmsType.h"

#include <QtCore/QHash>
#include <QtCore/QString>

class SmsCounter
{
public:
  SmsCounter( SmsType t, qint64 bytes ) : totalBytes{ bytes }, count{ 1 }, type{ t } {}

  void add( qint64 bytes )
  {
    ++count;
    this->totalBytes += bytes;
  }

  SmsType getType() const { return type; }
  int getCount() const { return count; }
  qint64 getBytes() const { return totalBytes; }

private:
  qint64 totalBytes{ 0 };
  int count{ 0 };
  SmsType type;
};

class TypeCounter
{
public:
  TypeCounter() = default;
  void add( SmsType, qint64 bytes );
  QVector<SmsCounter> getSortedCounters() const;
  QString toString() const;

private:
  QVector<SmsCounter> counters;
};
