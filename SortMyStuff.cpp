#include "DualPaneView.h"
#include "SmsMainWindow.h"
#include "SmsTree.h"

#include <QtCore/QCommandLineParser>
#include <QtCore/QTextStream>
#include <QtCore/QVector>
#include <QtWidgets/QApplication>

void showNode( QTextStream& out, SmsNode* node, const QString& prefix )
{
  out << prefix << node->getName() << " " << node->getSize() << "\n";
  for( const auto& child : node->getChildren() )
  {
    showNode( out, child, prefix + "  " );
  }
}

void showTypes( QTextStream& out, const SmsTree& tree, bool verbose )
{
  const auto sortedCounter = tree.getRoot()->getCounter().getSortedCounters();
  for( const auto& counter : sortedCounter )
  {
    out << QString{ "%1 %2 %3\n" }
               .arg( counter.getBytes(), 12 )
               .arg( counter.getCount(), 9 )
               .arg( nameOf( counter.getType() ) );
  }

  if( verbose )
  {
    QVector<const SmsNode*> unknownNodes;
    for( const SmsNode* node : tree.getNodes() )
    {
      if( node->getType() == SmsType::Unknown )
      {
        unknownNodes.append( node );
      }
    }
    std::sort( std::begin( unknownNodes ),
               std::end( unknownNodes ),
               []( const SmsNode* a, const SmsNode* b )
               { return a->getSize() > b->getSize(); } );
    int count{ 0 };
    for( const auto* node : unknownNodes )
    {
      out << QString{ "%1 %2\n" }.arg( node->getSize(), 12 ).arg( node->getName() );
    }
  }
  //
}

int main( int argc, char* argv[] )
{
  QApplication app( argc, argv );
  QCoreApplication::setOrganizationName( "cw3.de" );
  QCoreApplication::setApplicationName( "SortMyStuff" );
  QCommandLineParser parser;
  parser.addOptions( {
      { { "c", "check" }, "check directories" },
      { { "d", "dual" }, "dual-pane view" },
      { { "t", "types" }, "check types" },
      { { "g", "gui" }, "start gui" },
      { { "v", "verbose" }, "be more verbose" },
  } );
  parser.addHelpOption();
  parser.process( app );
  const bool verbose = parser.isSet( "verbose" );
  const auto args = parser.positionalArguments();

  if( parser.isSet( "gui" ) )
  {
    SmsMainWindow w;
    w.show();
    return app.exec();
  }
  else if( parser.isSet( "dual" ) )
  {
    DualPaneView w;
    w.setMinimumSize( 1200, 800 );
    w.show();
    return app.exec();
  }
  else if( parser.isSet( "check" ) )
  {
    QTextStream out{ stdout };
    for( const auto& arg : args )
    {
      SmsTree tree;
      tree.synchronousScan( arg );
      showNode( out, tree.getRoot(), "" );
    }
  }
  else if( parser.isSet( "types" ) )
  {
    QTextStream out{ stdout };
    for( const auto& arg : args )
    {
      SmsTree tree;
      tree.synchronousScan( arg );
      showTypes( out, tree, verbose );
    }
  }
  else
  {
    parser.showHelp();
  }
}
