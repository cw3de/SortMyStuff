#pragma once

#include "SmsTree.h"

#include <QAbstractItemModel>

class SmsModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit SmsModel( QObject* parent = nullptr );
  void startScanner( const QString& path );
  void cancelScanner();

  // Header:
  QVariant headerData( int section,
                       Qt::Orientation orientation,
                       int role = Qt::DisplayRole ) const override;

  // Basic functionality:
  QModelIndex
  index( int row, int column, const QModelIndex& parent = QModelIndex() ) const override;
  QModelIndex parent( const QModelIndex& index ) const override;

  int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
  int columnCount( const QModelIndex& parent = QModelIndex() ) const override;

  QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;

signals:
  void scanStarted();
  void scanProgress( int todo, int done, const QString& );
  void scanCanceled();
  void scanFinished();

private slots:
  void onScanCanceled();
  void onScanFinished();

private:
  SmsTree tree;
  SmsScanner* scanner{ nullptr };
};
