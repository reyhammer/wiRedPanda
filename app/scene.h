#ifndef SCENE_H
#define SCENE_H

#include "graphicelement.h"

#include <QGraphicsScene>
#include <QObject>

class Scene : public QGraphicsScene {
public:
  explicit Scene( QObject *parent = nullptr );
  Scene( const QRectF &sceneRect, QObject *parent = nullptr );
  Scene( qreal x, qreal y, qreal width, qreal height, QObject *parent = nullptr );

  /* QGraphicsScene interface */
  int gridSize( ) const;
  QVector< GraphicElement* > getElements( );
  QVector< GraphicElement* > getElements( QRectF rect );
  QVector< QNEConnection* > getConnections( );
  QVector< GraphicElement* > selectedElements( );

  void setDots( const QPen &dots );

  QVector< GraphicElement* > getVisibleElements( );
protected:
  void drawBackground( QPainter *painter, const QRectF &rect );
  static constexpr int m_gridSize = 16;
  QPen m_dots;
};

#endif /* SCENE_H */