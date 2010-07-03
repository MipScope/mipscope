/* ---------------------------------------------- *\
   file: scene.h
   auth: Travis Fischer, Tim O'Donnell
   acct: tfischer, tim
   date: 5/26/2007

   Modified version of Qt 'chip' demo.
\* ---------------------------------------------- */

#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

class Chip;
class View;
class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
class QGraphicsItem;

class Scene : public QGraphicsScene
{
   Q_OBJECT

   public:
      Scene(View *parentView);
           
   protected:
      void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
      View *m_view;
};

#endif

