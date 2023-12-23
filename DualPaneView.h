#pragma once
#include "DirectoryView.h"
#include "InfoCache.h"

#include <QMainWindow>

class DualPaneView : public QMainWindow
{
  Q_OBJECT
public:
  DualPaneView();
  ~DualPaneView() override = default;

private slots:
  void gotKeyPressed( QKeyCombination );

private:
  DirectoryView* leftView{ nullptr };
  DirectoryView* rightView{ nullptr };
  DirectoryView* activeView{ nullptr };
  InfoCache cache;

  virtual void keyPressEvent( QKeyEvent* event ) override;
  void updateSize( DirectoryView*, bool all );
  void moveCurrentItem( DirectoryView* from, DirectoryView* to, bool renameExisting );
  void showItem( DirectoryView* );
  void removeItem( DirectoryView* );
};
