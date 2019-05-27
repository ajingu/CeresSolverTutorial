#include <iostream>

#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include <glog/logging.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace ceres;

//camera coordinate
Point3d observed(0, 1, 0);

struct CostFunctor
{
	template <typename T>
	bool operator()(const T* const camera,  T* residual) const
	{
		//wordl coordinate
		const T world[3] = { T(0.8), T(0), T(0.6) };

		T p[3];

		//p = Rodrigues(camera) * world
		AngleAxisRotatePoint(camera, world, p);

		residual[0] = T(observed.x) - p[0];
		residual[1] = T(observed.y) - p[1];
		residual[2] = T(observed.z) - p[2];

		return true;
	}
};

int main(int argc, char** argv)
{
	google::InitGoogleLogging(argv[0]);

	
	Mat camera_rot = Mat::ones(3, 3, CV_64FC1);
	Mat camera_rvec;
	Rodrigues(camera_rot, camera_rvec);
	double* camera_rvec_data = (double*)camera_rvec.data;

	Problem problem;

	CostFunction* cost_function =
		new AutoDiffCostFunction<CostFunctor, 3, 3>(new CostFunctor);
	problem.AddResidualBlock(cost_function, NULL, camera_rvec_data);

	Solver::Options options;
	options.linear_solver_type = DENSE_SCHUR;
	options.minimizer_progress_to_stdout = true;
	Solver::Summary summary;
	Solve(options, &problem, &summary);

	cout << summary.BriefReport() << endl;


	Mat last_rvec(3, 1, CV_64FC1, camera_rvec_data);
	Mat last_rot;
	Rodrigues(last_rvec, last_rot);

	cout << "last rot: " << endl << last_rot << endl;

	system("PAUSE");

	return 0;
}