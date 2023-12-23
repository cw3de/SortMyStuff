#include "SmsCounter.h"

void TypeCounter::add( SmsType type, qint64 bytes )
{
  for( auto& cnt : counters )
  {
    if( cnt.getType() == type )
    {
      cnt.add( bytes );
      return;
    }
  }
  counters.append( SmsCounter{ type, bytes } );
}

QVector<SmsCounter> TypeCounter::getSortedCounters() const
{
  auto sorted = counters;
  std::sort( std::begin( sorted ),
             std::end( sorted ),
             []( const SmsCounter& a, const SmsCounter& b )
             { return a.getBytes() > b.getBytes(); } );

  return sorted;
}

QString TypeCounter::toString() const
{
  QString text;
  const auto sorted = getSortedCounters();
  for( const auto& cnt : sorted )
  {
    if( !text.isEmpty() )
      text.append( ", " );

    text.append( QString{ "%1:%2/%3" }
                     .arg( nameOf( cnt.getType() ) )
                     .arg( cnt.getCount() )
                     .arg( cnt.getBytes() ) );
  }
  return text;
}
