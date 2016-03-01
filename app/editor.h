#ifndef EDITOR_H
#define EDITOR_H


#include "simulationcontroller.h"
#include <QObject>
#include <QTime>
#include <QUndoCommand>
#include <elementeditor.h>
#include <elementfactory.h>
#include <memory>
#include <nodes/qneconnection.h>
#include <scene.h>

class Box;
class MainWindow;

class Editor : public QObject {
  Q_OBJECT
  QTime timer;
public:
  explicit Editor( MainWindow *parent = 0 );
  virtual ~Editor( );
  void install( Scene *s );
  void save( QDataStream &ds );
  void load( QDataStream &ds );
  void cut( QDataStream &ds );
  void copy( QDataStream &ds );
  void paste( QDataStream &ds );
  void selectAll( );
signals:
  void scroll( int x, int y );

public slots:
  void clear( );
  void showWires( bool checked );
  void showGates( bool checked );
  void rotate( bool rotateRight );
  void elementUpdated( GraphicElement *element, QByteArray itemData );

private:
  QUndoStack *undoStack;
  Scene *scene;
  QList< QGraphicsItem* > itemsAt( QPointF pos );
  QGraphicsItem* itemAt(QPointF pos );
  QNEConnection *editedConn;
  ElementFactory factory;
  ElementEditor *elementEditor;
  bool markingSelectionBox;
  QGraphicsRectItem *selectionRect;
  QPointF selectionStartPoint;
  SimulationController *simulationController;
  QPointF mousePos, lastPos;
  void addItem( QGraphicsItem *item );
  QNEPort *m_hoverPort;
  bool draggingElement;
  QList< GraphicElement* > movedElements;
  QList< QPointF > oldPositions;
  MainWindow *mainWindow;
  bool mControlKeyPressed;


  bool mousePressEvt( QGraphicsSceneMouseEvent *mouseEvt );
  bool mouseMoveEvt( QGraphicsSceneMouseEvent *mouseEvt );
  bool mouseReleaseEvt( QGraphicsSceneMouseEvent *mouseEvt );
  bool dropEvt( QGraphicsSceneDragDropEvent *dde );
  bool dragMoveEvt( QGraphicsSceneDragDropEvent *dde );
  bool wheelEvt( QWheelEvent *wEvt );

  void ctrlDrag( GraphicElement *elm, QPointF pos );

  /* QObject interface */
public:
  bool eventFilter( QObject *obj, QEvent *evt );
  void deleteElements( );
  void setElementEditor( ElementEditor *value );
  QUndoStack* getUndoStack( ) const;
  ElementFactory &getFactory( );
  Scene* getScene( ) const;
  void buildSelectionRect( );
  void handleHoverPort( QNEPort *port );
  void releaseHoverPort( );
  bool loadBox( Box *box, QString fname );
  void resizeScene( );
  bool getControlKeyPressed( ) const;
  void setControlKeyPressed( bool controlKeyPressed );
  SimulationController * getSimulationController() const;
};

#endif /* EDITOR_H */