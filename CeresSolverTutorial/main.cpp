#include <iostream>

#include <ceres/ceres.h>
#include <glog/logging.h>

using namespace std;

using ceres::AutoDiffCostFunction;
using ceres::CostFunction;
using ceres::Problem;
using ceres::Solver;
using ceres::Solve;

/**
 *  @brief �R�X�g�֐�
 **/
struct CostFunctor {
	template <typename T>
	bool operator()(const T* const x, T* residual) const {
		residual[0] = T(10.0) - x[0];
		return true;
	}
};


int main(int argc, char** argv) 
{
	google::InitGoogleLogging(argv[0]);

	//�œK�����������ϐ��Ə����l�̐ݒ�
	double initial_x = 5.0;
	double x = initial_x;

	//�œK�����������p�̃I�u�W�F�N�g�̐���
	Problem problem;

	//�R�X�g�֐��̐ݒ�
	//AutoDiffCostFunction���g�����ƂŁA�����I�Ƀ��R�r�s���ݒ�ł���
	CostFunction* cost_function = new AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor);

	//�œK�����Ɏc�����ƕϐ���ݒ�
	problem.AddResidualBlock(cost_function, NULL, &x);

	//�œK���̎��s
	Solver::Options options;//�œK���̃I�v�V�����ݒ�p�\����
	options.linear_solver_type = ceres::DENSE_QR;
	options.minimizer_progress_to_stdout = true;//�œK���̌��ʂ�W���o�͂ɕ\������B
	Solver::Summary summary;//�œK���̌��ʂ��i�[����p�\����
	Solve(options, &problem, &summary);//�œK���̎��s

	//���ʂ̕\��
	cout << summary.BriefReport() << endl;
	cout << "x:" << initial_x << "->" << x << endl;

	system("PAUSE");

	return 0;
}