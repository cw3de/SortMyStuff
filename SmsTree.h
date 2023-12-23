#pragma once

#include "SmsNode.h"

#include <QtCore/QMultiHash>
#include <QtCore/QObject>
#include <QtCore/QThread>

class SmsTree;

class SmsScanner : public QThread
{
  Q_OBJECT
public:
  SmsScanner( SmsTree*, const QString& rootPath );
  ~SmsScanner() override;
  void run() override;

signals:
  void scanStarted();
  void scanProgress( int todo, int done, const QString& );
  void scanCanceled();
  void scanFinished();

private:
  SmsTree* tree{ nullptr };
  QString rootPath;
};

class SmsTree : public QObject
{
  Q_OBJECT
  friend class SmsScanner;

public:
  SmsTree();
  ~SmsTree() override;

public slots:
  void clear();
  void synchronousScan( const QString& path );

public:
  SmsNode* getRoot() const { return root; }
  const QVector<SmsNode*>& getNodes() const { return allNodes; }

private:
  SmsNode* root{ nullptr };
  QVector<SmsNode*> allNodes;
};
