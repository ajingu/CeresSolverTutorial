#include <iostream>

#include <ceres/ceres.h>
#include <glog/logging.h>

using namespace std;
using namespace ceres;

struct CostFunctor 
{
	template <typename T>
	bool operator()(const T* const x, T* residual) const
	{
		residual[0] = T(10.0) - x[0];
		residual[1] = T(3.0) - x[1];

		return true;
	}
};

int main(int argc, char** argv) 
{
	google::InitGoogleLogging(argv[0]);

	double x[2] = { 0, 1 };
	double y[2] = { 6, 5 };

	Problem problem;

	CostFunction* cost_function =
		new AutoDiffCostFunction<CostFunctor, 2, 2>(new CostFunctor);
	problem.AddResidualBlock(cost_function, NULL, x);
	problem.AddResidualBlock(cost_function, NULL, y);

	Solver::Options options;
	options.linear_solver_type = DENSE_QR;
	options.minimizer_progress_to_stdout = true;
	Solver::Summary summary;
	Solve(options, &problem, &summary);

	cout << summary.BriefReport() << "\n";
	cout << "x : " << x[0] << " " << x[1] << "\n";
	cout << "y : " << y[0] << " " << y[1] << "\n";

	system("PAUSE");

	return 0;
}