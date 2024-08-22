#include "mpc_tracking/mpc.h"
#include <ros/ros.h>
#define PI 3.14159265358979323846
Mpc::Mpc() {
    mpc_window = 9; //要预测29个状态x1-x29  需要用到1个初始参考状态x0+29个参考输入(u0-u28)=(共30个状态)
    dt_ = 0.1;

    //cast the MPC problem as QP problem
    setWeightMatrices();
    castMPCToQPConstraintMatrix();
    castMPCToQPConstraintVectors();
    
}
Mpc::~Mpc() {}


void Mpc::solveMpc(Eigen::Vector3d x0, vector<Eigen::Vector3d> X_r, vector<Eigen::Vector2d> U_r) {
    // update x0
    castMPCToQPHessianAndGradient(x0, X_r, U_r, Q_, R_, hessian_, gradient_);

    // instantiate the solver
    OsqpEigen::Solver solver;

    //settings
    solver.settings()->setVerbosity(false);
    solver.settings()->setWarmStart(true);

    //set the initial data of the QP solver
    solver.data()->setNumberOfVariables(2 * mpc_window);
    solver.data()->setNumberOfConstraints(2 * mpc_window);
    if (!solver.data()->setHessianMatrix(hessian_)) return;
    if (!solver.data()->setGradient(gradient_)) return;
    if (!solver.data()->setLinearConstraintsMatrix(constraint_matrix_)) return;
    if (!solver.data()->setLowerBound(lower_bound_)) return;
    if (!solver.data()->setUpperBound(upper_bound_)) return;

    // instantiate the solver
    if (!solver.initSolver()) return;

    //controller input and QPSolution vector

    // solve the QP problem
    if (solver.solveProblem() != OsqpEigen::ErrorExitFlag::NoError) return;
    
    // get QP solution
    QP_solution_ = solver.getSolution();
    //cout << "QP_solution_ :" << QP_solution_.transpose()  << endl; 
 
}

Eigen::Vector2d Mpc::getControlCmd() {
    Eigen::Vector2d cmd;
    cmd = QP_solution_.block(0, 0, 2, 1);
    return cmd;
}

Eigen::MatrixXd Mpc::getPredictState(Eigen::Vector3d x_k, vector<Eigen::Vector3d> X_r, vector<Eigen::Vector2d> U_r) {
   Eigen::MatrixXd predict_states(3*mpc_window,1);
    vector<Eigen::MatrixXd> B_r(mpc_window), A_r(mpc_window);
    Eigen::MatrixXd O_r(3*mpc_window,1), X_ref(3*mpc_window,1), A_multiply1, A_multiply2;
    Eigen::MatrixXd A_bar(3*mpc_window,3), B_bar(3 * mpc_window, 2 * mpc_window), C_bar(3 * mpc_window, 3 * mpc_window);
    for (int i = 0; i < mpc_window; ++i) {
        B_r[i] = Eigen::MatrixXd::Zero(3,2);
        A_r[i] = Eigen::MatrixXd::Zero(3,3);

        A_r[i](0,2) = -U_r[i](0)*sin(X_r[i](2));
        A_r[i](1,2) = U_r[i](0)*cos(X_r[i](2));
        Eigen::Vector3d temp_vec = -dt_*A_r[i]*X_r[i];
        O_r.block<3,1>(i*3,0) = temp_vec;
        A_r[i] = Eigen::MatrixXd::Identity(3,3) + dt_*A_r[i];

        B_r[i](0,0) = cos(X_r[i](2))*dt_;
        B_r[i](1,0) = sin(X_r[i](2))*dt_;
        B_r[i](2,1) = dt_;

        X_ref.block<3,1>(i*3,0) = X_r[i+1]; //这个X_ref是做误差用的，不需要第一个参考状态,最后一个参考状态会被用到

        if(i==0) A_multiply1 = A_r[i];
        //else A_multiply1 = A_multiply1 * A_r[i]; //这里应该是A_r[i]*A_multiply1???
        else A_multiply1 = A_r[i]*A_multiply1; //这里应该是A_r[i]*A_multiply1???
        A_bar.block<3,3>(3*i,0) = A_multiply1;
    }
    B_bar = Eigen::MatrixXd::Zero(3*mpc_window,2*mpc_window);
    C_bar = Eigen::MatrixXd::Identity(3*mpc_window,3*mpc_window);
    for(int k = 0;k < mpc_window; k++)
    {
        B_bar.block<3,2>(3*k,2*k) = B_r[k];
        A_multiply2 = Eigen::MatrixXd::Identity(3,3);
        for(int i = 0;i < k; i++)
        {
            A_multiply2 = A_multiply2*A_r[k-i];
            C_bar.block<3,3>(3*k,3*(k-1-i)) = A_multiply2;
            B_bar.block<3,2>(3*k,2*(k-1-i)) = A_multiply2*B_r[k-1-i];
        }
    }
    predict_states = A_bar * x_k + B_bar * QP_solution_ + C_bar * O_r;
    return predict_states;
}



void Mpc::setWeightMatrices() {
    double omega0 = 60, omega1 = 10; 
    Q_ = Eigen::MatrixXd::Identity(3*mpc_window,3*mpc_window)*omega0;//对状态偏差进行较低的惩罚
    //for(int i=0; i<mpc_window; i+=1)
   //     Q_(i*3+2,i*3+2) = 0;
    R_ = Eigen::MatrixXd::Identity(2*mpc_window,2*mpc_window)*omega1;//对控制量大小进行较低的惩罚
}

void Mpc::castMPCToQPHessianAndGradient(Eigen::Vector3d x_k, vector<Eigen::Vector3d> X_r, vector<Eigen::Vector2d> U_r, const Eigen::MatrixXd &Q, const Eigen::MatrixXd &R,
                                         Eigen::SparseMatrix<double> &hessian_matrix, Eigen::VectorXd &gradient) { //判断搜索步长、确定停止条件

    hessian_matrix.resize(2 * mpc_window, 2 * mpc_window);
    //hessian_matrix = 2*(B_bar.transpose()*Q*B_bar+R); gradient = 2*B_bar.transpose()*Q*E; E = A_Bar*x_k + C_Bar*O - X_ref;
    //接下来求取B_bar（3 * mpc_window, 2 * mpc_window); gradient (2 * mpc_window, 1)
    //将B_bar再次划分为矩阵向量 vector<MatrixXd> B_r(mpc_window)
    //
    vector<Eigen::MatrixXd> B_r(mpc_window), A_r(mpc_window);
    Eigen::MatrixXd O_r(3*mpc_window,1), X_ref(3*mpc_window,1), A_multiply1, A_multiply2;
    Eigen::MatrixXd A_bar(3*mpc_window,3), B_bar(3 * mpc_window, 2 * mpc_window), C_bar(3 * mpc_window, 3 * mpc_window);
    for (int i = 0; i < mpc_window; ++i) {
        B_r[i] = Eigen::MatrixXd::Zero(3,2);
        A_r[i] = Eigen::MatrixXd::Zero(3,3);

        A_r[i](0,2) = -U_r[i](0)*sin(X_r[i](2));
        A_r[i](1,2) = U_r[i](0)*cos(X_r[i](2));
        Eigen::Vector3d temp_vec = -dt_*A_r[i]*X_r[i];
        O_r.block<3,1>(i*3,0) = temp_vec;
        A_r[i] = Eigen::MatrixXd::Identity(3,3) + dt_*A_r[i];

        B_r[i](0,0) = cos(X_r[i](2))*dt_;
        B_r[i](1,0) = sin(X_r[i](2))*dt_;
        B_r[i](2,1) = dt_;

        X_ref.block<3,1>(i*3,0) = X_r[i+1];

        if(i==0) A_multiply1 = A_r[i];
        //else A_multiply1 = A_multiply1 * A_r[i]; //这里应该是A_r[i]*A_multiply1???
        else A_multiply1 = A_r[i]*A_multiply1; //这里应该是A_r[i]*A_multiply1???
        A_bar.block<3,3>(3*i,0) = A_multiply1;
    }
    B_bar = Eigen::MatrixXd::Zero(3*mpc_window,2*mpc_window);
    C_bar = Eigen::MatrixXd::Identity(3*mpc_window,3*mpc_window);
    for(int k = 0;k < mpc_window; k++)
    {
        B_bar.block<3,2>(3*k,2*k) = B_r[k];
        A_multiply2 = Eigen::MatrixXd::Identity(3,3);
        for(int i = 0;i < k; i++)
        {
            A_multiply2 = A_multiply2*A_r[k-i];
            C_bar.block<3,3>(3*k,3*(k-1-i)) = A_multiply2;
            B_bar.block<3,2>(3*k,2*(k-1-i)) = A_multiply2*B_r[k-1-i];
        }
    }
   
    Eigen::MatrixXd E = A_bar*x_k+C_bar*O_r-X_ref;
  //  hessian_matrix = 2*(B_bar.transpose()*Q*B_bar+R);      ////Hesse矩阵：稀疏矩阵 不能直接赋值
    gradient = 2*B_bar.transpose()*Q*E;       ////一次项系数

    // 计算出我们需要的临时密集矩阵
    Eigen::MatrixXd temp_dense_matrix = 2 * (B_bar.transpose() * Q * B_bar + R);

    // 预备一个三元组向量以存储矩阵中的元素
    std::vector<Eigen::Triplet<double>> triplets;
    triplets.reserve(temp_dense_matrix.rows() * temp_dense_matrix.cols());
    
    // 遍历整个密集矩阵
    for (int i = 0; i < temp_dense_matrix.rows(); ++i) {
        for (int j = 0; j < temp_dense_matrix.cols(); ++j) {
            double value = temp_dense_matrix(i, j);
            // 只保留非零元素
            if (std::abs(value) > 1e-10) {
                triplets.push_back({i, j, value});
            }
        }
    }
    // 最后，使用提取的非零元素的三元组列表来填充稀疏矩阵
    hessian_matrix.setFromTriplets(triplets.begin(), triplets.end());
}


void Mpc::castMPCToQPConstraintMatrix() {
    //constraint_matrix_ = Eigen::MatrixXd::Identity(2 * mpc_window, 2 * mpc_window);
    // 设置对角元素
    constraint_matrix_.resize(2 * mpc_window, 2 * mpc_window);
    for (int i = 0; i < mpc_window; ++i) {
    // 设置对角线上的元素
    constraint_matrix_.coeffRef(i, i) = 1.0; // 对角线上的元素为1.0
    constraint_matrix_.coeffRef(i + mpc_window, i + mpc_window) = 1.0; // 对角线上的元素为1.0
    }

    // 调用finalize()方法
    constraint_matrix_.makeCompressed();
}


void Mpc::castMPCToQPConstraintVectors() {
    // evaluate the lower and the upper inequality vectors
    u_max_ << 0.5, 0.8;
    u_min_ << -0.5, -0.8;
    lower_bound_ = Eigen::MatrixXd::Zero(2 * mpc_window, 1);
    upper_bound_ = Eigen::MatrixXd::Zero(2 * mpc_window, 1); 

    for (int i = 0; i < 2 * mpc_window; ++i) {
        if(i%2==0)
        {
            lower_bound_[i] = u_min_[0];
            upper_bound_[i] = u_max_[0];
        }
        else
        {
            lower_bound_[i] = u_min_[1];
            upper_bound_[i] = u_max_[1];
        }
    }                                       
  
}