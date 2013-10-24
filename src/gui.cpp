#include "gui.hh"

OknoGlowne::OknoGlowne( QWidget *wRodzic ): QMainWindow( wRodzic ){

  W = new Wizualizacja( this );
  S = new Wahadlo( this );
  B = new QPushButton( this );
  resize(600,400);
  setCentralWidget( W );

  B->resize( 100, 30 );
  B->move( 500, 300 );
  B->setText("Apply Force");

  connect(S, SIGNAL(UpdateAngles(Parameters)), W, SLOT(fresh(Parameters)));
  connect(B, SIGNAL(pressed()), S, SLOT(ApplyForce()));
  connect(B, SIGNAL(released()), S, SLOT(ReleaseForce()));

}

Wizualizacja::Wizualizacja( QWidget *wRodzic ): QWidget( wRodzic ){

  param.q[0] = -90; param.q[1] = -90; param.q[2] = 0; param.q[3] = 0;
  param.l[0] = 6; param.l[1] = 3;
  param.m[0] = 5; param.m[1] = 4;

  setPalette( QPalette(Qt::white) );
  setAutoFillBackground( true );
  update();

}

void Wizualizacja::paintEvent( QPaintEvent * ){

  QPainter Rys( this );
  Rys.setBackground( Qt::white );

  QLineF *lines = new QLineF[2];

  lines[0] = QLineF::fromPolar(param.l[0]*10,param.q[0]);
  lines[1] = QLineF::fromPolar(param.l[1]*10,param.q[0]+param.q[1]);

  for( int i=1; i < 2; i++ ) lines[i].translate(lines[i-1].p2());

  for( int i=0; i < 2; i++ ){

    lines[i].translate(200,100);
    Rys.drawLine( lines[i] );

  }

  delete[] lines;

}

void Wizualizacja::fresh( Parameters p ){

  for(int i=0; i < 4; i++){

    param.q[i] = -p.q[i]*180/PI; // in deg

  }

  param.l[0] = p.l[0]; param.l[1] = p.l[1];
  param.m[0] = p.m[0]; param.m[1] = p.m[1];
  update();

}
