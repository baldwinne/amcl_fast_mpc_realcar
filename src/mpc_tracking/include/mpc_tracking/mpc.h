#ifndef MPC_H
#define MPC_H

#include <vector>
//#include <Eigen/Dense>
#include </usr/include/eigen3/Eigen/Dense>
#include </usr/include/eigen3/Eigen/Core>
#include "tf2/utils.h"
#include <iostream>
#include <chrono>
#include "OsqpEigen/OsqpEigen.h"

using namespace std;

class Mpc
{
private:
    //preview window
    int mpc_window;
    double dt_;


    //constraints vector
    Eigen::Vector2d u_max_, u_min_;

    //weight matrices
    Eigen::MatrixXd Q_, R_;

    //the initial and the reference state
    Eigen::Vector2d x0_, x_ref_;

    //QP problem matrices and vectors
    Eigen::SparseMatrix<double> hessian_;
    Eigen::VectorXd gradient_;
    Eigen::SparseMatrix<double> constraint_matrix_;
    //Eigen::VectorXd constraint_matrix_;
    Eigen::VectorXd lower_bound_, upper_bound_; 

    // QP solution
    Eigen::VectorXd QP_solution_;

    
 

public:
    Mpc();
    ~Mpc();


    void setWeightMatrices();
    void castMPCToQPHessianAndGradient(Eigen::Vector3d x_k, vector<Eigen::Vector3d >X_r, vector<Eigen::Vector2d >U_r, const Eigen::MatrixXd &Q, const Eigen::MatrixXd &R,
                                         Eigen::SparseMatrix<double> &hessian_matrix, Eigen::VectorXd &gradient);
    
    void castMPCToQPConstraintMatrix();
    void castMPCToQPConstraintVectors();
    
    void solveMpc(Eigen::Vector3d x0, vector<Eigen::Vector3d> X_r, vector<Eigen::Vector2d> U_r);
    Eigen::Vector2d getControlCmd();
    Eigen::MatrixXd getPredictState(Eigen::Vector3d x_k, vector<Eigen::Vector3d> X_r, vector<Eigen::Vector2d> U_r);
    
};

#endif