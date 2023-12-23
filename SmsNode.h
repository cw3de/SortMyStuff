#pragma once

#include "SmsCounter.h"
#include "SmsType.h"

#include <QtCore/QString>
#include <QtCore/QVector>

class SmsNode
{
public:
  SmsNode( const QString& path, const QString& name, qint64 totalSize, SmsNode* parent );
  SmsNode( const QString& name, qint64 size, SmsType, SmsNode* parent );

  SmsNode() = default;
  SmsNode( const SmsNode& ) = default;
  SmsNode& operator=( const SmsNode& ) = default;
  ~SmsNode() = default;

  SmsNode* getParent() const { return parent; }
  const QVector<SmsNode*>& getChildren() const { return children; }
  const QString& getName() const { return name; }
  const QString& getPath() const { return path; }
  qint64 getSize() const { return size; }
  const TypeCounter& getCounter() const { return typeCounter; }
  SmsType getType() const { return type; }
  bool isDir() const { return type == SmsType::Directory; }

  void addChild( SmsNode* );

private:
  SmsNode* parent{ nullptr };
  QVector<SmsNode*> children;
  QString name;
  QString path;
  qint64 size;
  TypeCounter typeCounter;
  SmsType type;
};
