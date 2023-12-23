#include "SmsNode.h"

#include <QtCore/QFile>

SmsNode::SmsNode( const QString& p, const QString& n, qint64 s, SmsNode* par )
    : parent{ par }, name{ n }, path{ p }, size{ s }, type{ SmsType::Directory }
{
}

SmsNode::SmsNode( const QString& n, qint64 s, SmsType t, SmsNode* p )
    : parent{ p }, name{ n }, size{ s }, type{ t }
{
}

void SmsNode::addChild( SmsNode* child )
{
  children.append( child );
  if( child->size )
  {
    auto sumNode = this;
    while( sumNode )
    {
      sumNode->typeCounter.add( child->type, child->size );
      sumNode->size += child->size;
      sumNode = sumNode->parent;
    }
  }
}
