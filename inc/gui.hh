#ifndef GUI
#define GUI


#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QMainWindow>
#include <QPushButton>
#include <QBrush>
#include <QPalette>
#include <QLineEdit>
#include <QString>
#include <QLineF>
#include <iostream>
#include "symulacja.hh"



class Wizualizacja: public QWidget {
Q_OBJECT
  Parameters param;

public:

  Wizualizacja( QWidget *wRodzic = 0 );
  void paintEvent( QPaintEvent *event );

public slots:

  void fresh( Parameters p );

};


class OknoGlowne: public QMainWindow{

  Wizualizacja *W;
  Wahadlo *S;
  QPushButton *B;

public:

  OknoGlowne( QWidget *wRodzic = 0 );

};



#endif
