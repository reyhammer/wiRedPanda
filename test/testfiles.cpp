#include "testfiles.h"

#include "globalproperties.h"
#include <commands.h>
#include <mainwindow.h>
#include <stdexcept>
void TestFiles::init( ) {
  editor = new Editor( this );
}

void TestFiles::cleanup( ) {
  delete editor;
}

void TestFiles::testFiles( ) {
/*  qDebug( ) << "CURRENTDIR: " << CURRENTDIR; */
  QDir examplesDir( QString( "%1/../examples/" ).arg( CURRENTDIR ) );
/*  qDebug( ) << "Examples dir:" << examplesDir.absolutePath( ); */
  QStringList entries;
  entries << "*.panda";
  QFileInfoList files = examplesDir.entryInfoList( entries );
  QVERIFY( files.size( ) > 0 );
/*  int counter = 0; */
  for( QFileInfo f : files ) {
/*    qDebug( ) << "File " << counter++ << " from " << files.size( ) << ": " << f.fileName( ); */
    QVERIFY( f.exists( ) );
    QFile pandaFile( f.absoluteFilePath( ) );
    GlobalProperties::currentFile = f.absoluteFilePath( );
    QVERIFY( pandaFile.exists( ) );
    QVERIFY( pandaFile.open( QFile::ReadOnly ) );


    QDataStream ds( &pandaFile );
    try {
      editor->load( ds );
    }
    catch( std::runtime_error &e ) {
      QFAIL( QString( "Could not load the file! Error: %1" ).arg( QString::fromStdString( e.what( ) ) ).toUtf8( ) );
    }


    QList< QGraphicsItem* > items = editor->getScene( )->items( );
    for( QGraphicsItem *item : items ) {
      if( item->type( ) == QNEConnection::Type ) {
        QNEConnection *conn = qgraphicsitem_cast< QNEConnection* >( item );
        QVERIFY( conn != nullptr );
        QVERIFY( conn->start( ) != nullptr );
        QVERIFY( conn->end( ) != nullptr );
      }
    }

    pandaFile.close( );
    QString outFileName = QDir::temp().absoluteFilePath(f.fileName());

    qDebug() << outFileName;
    QFile outfile( outFileName );
    if( outfile.open( QFile::WriteOnly ) ) {
      QDataStream ds( &outfile );
      try {
        editor->save( ds );
      }
      catch( std::runtime_error &e ) {
        QFAIL(QString("Error saving project: " + outFileName).toUtf8());
      }
    }
    else {
      QFAIL(QString("Could not open file in WriteOnly mode : " + outFileName).toUtf8());
    }
    outfile.flush( );
    outfile.close( );

    QFile pandaFile2(outFileName);
    QVERIFY( pandaFile2.open( QFile::ReadOnly ) );
    QDataStream ds2( &pandaFile2 );
    try {
      editor->load( ds2 );
    }
    catch( std::runtime_error &e ) {
      QFAIL( QString( "Could not load the file! Error: %1" ).arg( QString::fromStdString( e.what( ) ) ).toUtf8( ) );
    }

  }
}
