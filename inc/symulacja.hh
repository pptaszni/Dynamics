/*!
 * \file symulacja.hh
 * \author Pawel Ptasznik
 *
 * \brief ODESolver class and some dynamic examples
 *
 * This file contains ODESolver class, that is the basis for
 * solving the Oridinary Differential Equations, espacially for dynamic
 * purposes. By default ODESolver solves the equation of the form
 * x' = x + u
 * The example class Przyklad solves the harmonic oscillator equation.
 * In this version only the Euler Method is available, however it is possible
 * to implement some additional features like Runge-Kutta.
 *
 */


#ifndef SYMULACJA
#define SYMULACJA

#include <QObject>
#include <QTimer>
#include <QMetaType>
#include <iostream>
#include <cmath>

#define PI 3.141592

using namespace std;

/*! \struct Parameters symulacja.hh "inc/symulacja.hh"
 * \brief Structure used for exchanging data between solver and graphical
 * engine
 */
struct Parameters{

  /*! \brief q1 - joint1, q2 - joint2, q3 = dq1/dt, q4 = dq2/dt */
  double q[4];
  /*! \brief l1 - joint1 length, l2 - joint2 length */
  double l[2];
  /*! \brief m1 - joint1 mass, m2 - joint2 mass */
  double m[2];

};

Q_DECLARE_METATYPE( Parameters )

/*! \class ODESolver symulacja.hh "inc/symulacja.hh"
 *
 * \brief Oridinary Differential Equations Solver class. To be inherited by
 * classes representing particullary ODEs.
 *
 * This class provides ODE Solver. In this version only Euler Method is
 * available, however more exact methods are meant to be implemented.
 * The class coltrols the basic parameters such as step size, initial
 * conditions, control imputs, and the dimensions of the problem. In order
 * to use this class it must be inherited and the virtual function ODEFun
 * must be reimplemented. Valid equations are of the form x' = f(x,u).
 */
class ODESolver{

  /*! \brief solver step */
  double step;
  /*! \brief initial conditions and further results - state vector */
  double *x0;
  /*! \brief control input vector*/
  double *u;
  /*! \brief x vector dimension */
  int dimx;
  /*! \brief control input vector dimension */
  int dimu;

public:

  /*! \brief step=0, x0=null, u=null, dim=0 */
  ODESolver();
  /*! \brief must be reimplemented in child class */
  virtual ~ODESolver();
  /*! \brief sets x dimension, should be called before other actions */
  void SetDimX(int dim);
  /*! \brief sets u dimension, should be called before other actions */
  void SetDimU(int dim);
  /*! \brief defines step size, should be called before starting the solver */
  void SetStep(double s);
  /*! \brief sets initial conditions, must be called before starting the solver */
  void SetInitialConditions(double *x);
  /*! \brief sets control inputs, must be called before starting the solver */
  void SetControlInput(double *uu);
  /*! \brief returns state vector dimension */
  int DimX();
  /*! \brief returns control vector dimension */
  int DimU();
  /*! \brief returns state vector */
  double* X();
  /*! \brief ODE Function
   *
   * This function describes the ODE system. It should be of the form
   * x' = f(x,u). Dimensions of x,u,xp are defined by parameters dimx, dimu.
   *
   * \param x state vector
   * \param u control input vector
   * \param xp first derivative of x, this parameter works as an output
   *
   * \pre for example system of equations 
   *
   * x1' = x2
   * x2' = -u
   *
   * should be writen:
   *
   * xp[0] = x[1]; xp[1] = -u[0]
   */
  virtual void ODEFun(double *x, double *u, double *xp);
  /*! \brief Performs a single integrating step using Euler Method. Saves the result in x0 vector. In order to solve the ODE on some time interval this method should be used in loop.
   */
  void EulerMethodStep();

};

class Przyklad: public ODESolver{

public:

  Przyklad();
  ~Przyklad();

  void ODEFun(double *x, double *u, double *xp);

};

class Wahadlo: public QObject, public ODESolver{
Q_OBJECT
  QTimer *Timer;
  Parameters param;

public:

  Wahadlo( QObject *wRodzic = 0 );
  ~Wahadlo();
  void  ODEFun(double *x, double *u, double *xp);

public slots:

  void DoTheStep();
  void ApplyForce();
  void ReleaseForce();

signals:

  void UpdateAngles( Parameters p );

};



#endif
