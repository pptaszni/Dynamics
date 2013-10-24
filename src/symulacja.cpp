#include "symulacja.hh"


///////////////////////////////////////////////////////////////////////////


ODESolver::ODESolver(): step(0), x0(NULL), u(NULL), dimx(0), dimu(0){}
ODESolver::~ODESolver(){

  if(x0 != NULL) delete[] x0;
  if(u != NULL) delete[] u;

}

void ODESolver::SetDimX(int dim){ dimx = dim; }
void ODESolver::SetDimU(int dim){ dimu = dim; }
void ODESolver::SetStep(double s){ step = s; }

void ODESolver::SetInitialConditions(double *x){

  if( dimx == 0 ){
    cout << "Dimx = 0\n";
    return;
  }

  if(x0 == NULL) x0 = new double[dimx];

  for(int i=0; i<dimx; i++)  x0[i] = x[i];

}

void ODESolver::SetControlInput(double *uu){

  if( dimu == 0 ){
    cout << "Dimu = 0\n";
    return;
  }

  if(u == NULL) u = new double[dimu];

  for(int i=0; i<dimu; i++)  u[i] = uu[i];

}

int ODESolver::DimX(){ return dimx; }
int ODESolver::DimU(){ return dimu; }
double* ODESolver::X(){ return x0; }
/* Example: x' = x; */
void ODESolver::ODEFun(double *x, double *u, double *xp){ 

  for(int i=0; i<dimx; i++) xp[i] = x[i]+u[0];

 }

void ODESolver::EulerMethodStep(){

  double *x_temp = new double[dimx];
  double *xp = new double[dimx];

  this->ODEFun(x0,u,xp);

  for(int i=0; i<dimx; i++) x_temp[i] = x0[i] + step*xp[i];
  for(int i=0; i<dimx; i++) x0[i] = x_temp[i];

  delete[] x_temp;
  delete[] xp;

}

/////////////////////////////////////////////////////////////////////////////



Przyklad::Przyklad(){

  /* Rownanie oscylatora x'' + 4x == 0, x(0)=1, x'(0)=0 */
  SetDimX(2);
  double *x0 = new double[2];
  x0[0]=1;x0[1]=0;
  SetInitialConditions(x0);
  delete[] x0;
  SetStep(0.01);

}
Przyklad::~Przyklad(){}

void Przyklad::ODEFun(double *x, double *u, double *xp){

  xp[0] = x[1];
  xp[1] = -4*x[0]+u[0];
  u;

}


Wahadlo::Wahadlo( QObject *wRodzic ): QObject( wRodzic ){

  param.q[0] = PI/2; param.q[1] = 0; param.q[2] = PI/2; param.q[3] = 0;
  param.l[0] = 6; param.l[1] = 3;
  param.m[0] = 5; param.m[1] = 4;

  SetDimX(4); // 4 states: q1,q2,q'1,q'2
  SetDimU(2); // u1, u2 - control inputs
  double *q0 = new double[4];
  double *u = new double[2];
  for(int i=0;i<4;i++) q0[i] = param.q[i];
  u[0] = 0; u[1] = 0;
  SetInitialConditions(q0);
  SetControlInput(u);
  delete[] q0;
  delete[] u;
  SetStep(0.01); // 10msec

  Timer = new QTimer( this );
  connect(Timer, SIGNAL(timeout()), this, SLOT(DoTheStep()));
  Timer->start(10); // 10msec

}

Wahadlo::~Wahadlo(){}

/* Euler-Lagrange equations */

void Wahadlo::ODEFun(double *x, double *u, double *xp){

  double g = 9.81;
  double l1 = param.l[0];
  double l2 = param.l[1];
  double m1 = param.m[0];
  double m2 = param.m[1];
  double u1 = u[0];
  double u2 = u[1];
  double Tarc = 50; // friction force

  xp[0] = x[2];
  xp[1] = x[3];
  xp[2] = (g*l1*l2*((2*m1 + m2)*cos(x[0]) - m2*cos(x[0] + 2*x[1])) + pow(l1,2)*l2*m2*sin(2*x[1])*pow(x[2],2) + 2*l1*pow(l2,2)*m2*sin(x[1])*pow(x[2] + x[3],2) - 2*l1*cos(x[1])*(u2 - Tarc*x[3]) + 2*l2*(u1 - u2 + Tarc*(-x[2] + x[3])))/(2.*pow(l1,2)*l2*(m1 + m2 - m2*pow(cos(x[1]),2)));
  xp[3] = ((g*l2*m2*cos(x[0] + x[1]) + u2 - l1*l2*m2*sin(x[1])*pow(x[2],2) - Tarc*x[3])/m2 - ((l2 + l1*cos(x[1]))*(g*l1*l2*((2*m1 + m2)*cos(x[0]) - m2*cos(x[0] + 2*x[1])) + pow(l1,2)*l2*m2*sin(2*x[1])*pow(x[2],2) + 2*l1*pow(l2,2)*m2*sin(x[1])*pow(x[2] + x[3],2) - 2*l1*cos(x[1])*(u2 - Tarc*x[3]) + 2*l2*(u1 - u2 + Tarc*(-x[2] + x[3]))))/(2.*pow(l1,2)*(m1 + m2 - m2*pow(cos(x[1]),2))))/pow(l2,2);

}

void Wahadlo::DoTheStep(){

  double *q;
  q = X();
  //cout << "q1="<<q[0]<<", q2="<<q[1]<<", q3="<<q[2]<<", q4="<<q[3]<<endl;
  for(int i=0; i<4; i++) param.q[i] = q[i];
  emit UpdateAngles( param );
  EulerMethodStep();

}

void Wahadlo::ApplyForce(){

  double *u = new double[2];
  u[0] = 1000;
  u[1] = 0;
  SetControlInput(u);
  delete[] u;

}

void Wahadlo::ReleaseForce(){

  double *u = new double[2];
  u[0] = 0;
  u[1] = 0;
  SetControlInput(u);
  delete[] u;

}
