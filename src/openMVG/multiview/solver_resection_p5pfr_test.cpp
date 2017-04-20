// Copyright (c) 2010 libmv authors.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.


// Copyright (c) 2012, 2013 Pierre MOULON.

// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <vector>
#include "openMVG/multiview/solver_resection_p5pfr.hpp"
#include "testing/testing.h"

using namespace std;
using namespace openMVG;

bool checkNear(Mat &expect, Mat &actual, double eps) {
	if (expect.rows() != actual.rows() || expect.cols() != actual.cols())
		return false;
	for (int i = 0; i < expect.rows(); ++i) {
		for (int j = 0; j < expect.cols(); ++j) {
			if (abs(expect(i, j) - actual(i, j)) > (eps * abs(expect(i, j))))
				return false;
		}
	}
	return true;
}

bool checkNear(Vec &expect, Vec &actual, double eps) {
	if (expect.rows() != actual.rows())
		return false;
	for (int i = 0; i < expect.rows(); ++i) {
			if (abs(expect(i) - actual(i)) >(eps * abs(expect(i))))
				return false;
	}
	return true;
}

bool checkNear(Vec3 &expect, Vec3 &actual, double eps) {
	if (expect.rows() != actual.rows())
		return false;
	for (int i = 0; i < expect.rows(); ++i) {
		if (abs(expect(i) - actual(i)) >(eps * abs(expect(i))))
			return false;
	}
	return true;
}

bool checkNear(double expect, double actual, double eps) {
	if (abs(expect - actual) > (eps * abs(expect)))
		return false;
	return true;
}

bool sortM(resection::M &i, resection::M &j) { return (i._f < j._f); }

bool testSolutions(std::vector<resection::M> &solutions, std::vector<resection::M> &models, double eps) {
	if (models.size() != solutions.size()) 
		return false;
	
	for (int i = 0; i < models.size(); ++i) {
		if ( !checkNear(solutions.at(i)._f, models.at(i)._f, eps) ) 
			return false;
		if ( !checkNear(solutions.at(i)._R, models.at(i)._R, eps) )
			return false;
		if ( !checkNear(solutions.at(i)._t, models.at(i)._t, eps) )
			return false;
		if ( !checkNear(solutions.at(i)._r, models.at(i)._r, eps) )
			return false;
	}
	return true;
}


TEST(Resection_P5Pfr, RandomRealExample) {
	// DATA
	Mat pt2D = Mat(2, 5);
	Mat pt3D = Mat(3, 5);
	pt2D << 4.803799999999999e+02, 5.422099999999998e+02, 1.508150000000000e+03, -1.640501000000000e+03, -4.026700000000001e+02, -1.326581000000000e+03, -7.138680000000001e+02, -1.234743000000000e+03, -6.222770000000000e+02, -1.461412100000000e+03;
	pt3D << 3.175360000000000e+00, 2.531220000000000e+00, 2.420890000000000e+00, 7.578520000000000e-01, 3.880860000000000e+00, -4.679980000000000e-01, 1.964960000000000e-01, -1.863250000000000e+00, 1.648030000000000e-01, 4.177460000000000e-01, -8.605090000000000e-01, -1.507400000000000e+00, -1.828080000000000e+00, 1.208810000000000e+00, 2.980880000000000e-01;

	// SOLUTIONS
	std::vector<resection::M> solutions;
	Mat R1 = Mat(3, 3);     R1 << -9.430073299811607e-01, 2.928814581476584e-01, -1.579798312288744e-01, -3.071682103315301e-01, -5.834954588038342e-01, 7.517850358427121e-01, 1.280033834065588e-01, 7.574651813968776e-01, 6.402043680012413e-01;
	Vec3 t1;                t1 << 3.351461753495843e+00, 3.662505952393360e-01, 1.522470547430807e+00;
	double                  f1 = 8.754207183076484e+03;
	Vec r1 = Vec(1);        r1 << -2.974934229711877e+01;
	resection::M m1(R1, t1, r1, f1);
	solutions.push_back(m1);
	Mat R2 = Mat(3, 3);     R2 << -9.288195037630559e-01, 2.583819805318196e-01, -2.655806498331628e-01, -3.682532345104426e-01, -5.643279703727857e-01, 7.388663594504280e-01, 4.103516421250264e-02, 7.840744186564097e-01, 6.193088254713202e-01;
	Vec3 t2;                t2 << 3.273710373948777e+00, 3.480925794418662e-01, 1.335812819189698e+00;
	double                  f2 = 6.523300619965637e+03;
	Vec r2 = Vec(1);        r2 << -1.716688911283097e+01;
	resection::M m2(R2, t2, r2, f2);
	solutions.push_back(m2);
	Mat R3 = Mat(3, 3);     R3 << 6.261772405396446e-01, 7.517419626249591e-01, -2.068479757187163e-01, 7.785113680902823e-01, -6.173581864945597e-01, 1.130881042477301e-01, -4.268621773305278e-02, -2.318466976191867e-01, -9.718153094177535e-01;
	Vec3 t3;                t3 << -2.286549972461777e+00, -1.360137887842060e+00, -5.183599359915888e-01;
	double                  f3 = 7.215366134863186e+03;
	Vec r3 = Vec(1);        r3 << -1.884020746278653e+01;
	resection::M m3(R3, t3, r3, f3);
	solutions.push_back(m3);
	Mat R4 = Mat(3, 3);     R4 << -2.156527230855822e-01, -6.509050028038228e-01, -7.278850048948177e-01, -8.734050807538640e-01, 4.619070646021064e-01, -1.542900793440778e-01, 4.366434105069141e-01, 6.024653857240441e-01, -6.681152528308791e-01;
	Vec3 t4;                t4 << 5.861602062248285e-01, 5.581995729744164e-01, 8.108474186437322e-01;
	double                  f4 = 1.770851635941407e+03;
	Vec r4 = Vec(1);        r4 << -2.945748397114161e-01;
	resection::M m4(R4, t4, r4, f4);
	solutions.push_back(m4);
	std::sort(solutions.begin(), solutions.end(), sortM);

	// PROCESS
	std::vector<resection::M> models;
	resection::P5PfrSolver::Solve(pt2D, pt3D, 1, &models);
	std::sort(models.begin(), models.end(), sortM);

	// TEST
	double eps = 1e-4;
	EXPECT_TRUE(testSolutions(solutions, models, eps));
}

TEST(Resection_P5Pfr, Test01) {
	// DATA
	Mat pt2D = Mat(2, 5);
	Mat pt3D = Mat(3, 5);
	pt2D << 7.566900000000000e+00, 1.153300000000000e+01, -6.677100000000000e+00, -8.057900000000000e+00, 9.185599999999999e-01, 9.171400000000000e+00, -8.827500000000001e+00, -8.109500000000001e+00, 6.638800000000000e+00, 4.434400000000000e-02;
	pt3D << 9.000000000000000e+00, 1.000000000000000e+01, -1.000000000000000e+01, -1.100000000000000e+01, 0.000000000000000e+00, 1.000000000000000e+01, -1.000000000000000e+01, -9.000000000000000e+00, 1.000000000000000e+01, 0.000000000000000e+00, 1.100000000000000e+01, 9.000000000000000e+00, 1.000000000000000e+01, 1.100000000000000e+01, 7.000000000000000e+00;

	// SOLUTIONS
	std::vector<resection::M> solutions;
	Mat R1 = Mat(3, 3);     R1 << -9.866769718754666e-01, 8.864003655023664e-02, 1.364239608391149e-01, -8.610773932011366e-02, -9.959878698575466e-01, 2.436432485840338e-02, 1.380362647996501e-01, 1.229255941612770e-02, 9.903508886163336e-01;
	Vec3 t1;                t1 << -2.338498898349219e+00, -2.373410185570847e-01, -2.137124572474679e+02;
	double                  f1 = 1.194729635360126e+02;
	Vec r1 = Vec(1);        r1 << 4.426177298997219e+01;
	resection::M m1(R1, t1, r1, f1);
	solutions.push_back(m1);
	Mat R2 = Mat(3, 3);     R2 << 9.911472004734465e-01, -9.612106035721957e-02, 9.158585452706794e-02, 9.612196425399246e-02, 9.953597879541551e-01, 4.411407010652809e-03, -9.158490586119367e-02, 4.431058526264710e-03, 9.957874124223167e-01;
	Vec3 t2;                t2 << -4.762130757439627e-04, 4.667150046849628e-05, 4.194470650574878e-04;
	double                  f2 = 1.000134624927844e+01;
	Vec r2 = Vec(1);        r2 << -1.000869226503413e-01;
	resection::M m2(R2, t2, r2, f2);
	solutions.push_back(m2);
	std::sort(solutions.begin(), solutions.end(), sortM);

	// PROCESS
	std::vector<resection::M> models;
	resection::P5PfrSolver::Solve(pt2D, pt3D, 1, &models);
	std::sort(models.begin(), models.end(), sortM);

	// TEST
	double eps = 1e-4;
	EXPECT_TRUE(testSolutions(solutions, models, eps));
}

TEST(Resection_P5Pfr, Test02) {
	// DATA
	Mat pt2D = Mat(2, 5);
	Mat pt3D = Mat(3, 5);
	pt2D << 7.566900000000001e+02, 1.153300000000000e+03, -6.677100000000000e+02, -8.057900000000000e+02, 9.185600000000000e+01, 9.171400000000000e+02, -8.827500000000000e+02, -8.109500000000001e+02, 6.638800000000000e+02, 4.434400000000000e+00;
	pt3D << 9.000000000000000e+00, 1.000000000000000e+01, -1.000000000000000e+01, -1.100000000000000e+01, 0.000000000000000e+00, 1.000000000000000e+01, -1.000000000000000e+01, -9.000000000000000e+00, 1.000000000000000e+01, 0.000000000000000e+00, 1.100000000000000e+01, 9.000000000000000e+00, 1.000000000000000e+01, 1.100000000000000e+01, 7.000000000000000e+00;

	// SOLUTIONS
	std::vector<resection::M> solutions;
	Mat R1 = Mat(3, 3);     R1 << -9.866769718754656e-01, 8.864003655023635e-02, 1.364239608391232e-01, -8.610773932011311e-02, -9.959878698575468e-01, 2.436432485840404e-02, 1.380362647996585e-01, 1.229255941612769e-02, 9.903508886163327e-01;
	Vec3 t1;                t1 << -2.338498898349279e+00, -2.373410185570856e-01, -4.243667543424492e+03;
	double                  f1 = 2.521752759667783e+05;
	Vec r1 = Vec(1);        r1 << 1.888402757691173e+04;
	resection::M m1(R1, t1, r1, f1);
	solutions.push_back(m1);
	Mat R2 = Mat(3, 3);     R2 << 9.911472004734461e-01, -9.612106035721975e-02, 9.158585452707382e-02, 9.612196425399260e-02, 9.953597879541551e-01, 4.411407010652902e-03, -9.158490586119952e-02, 4.431058526265199e-03, 9.957874124223163e-01;
	Vec3 t2;                t2 << -4.762130758018528e-04, 4.667150046743732e-05, 4.194544910240924e-04;
	double                  f2 = 1.000134625367708e+03;
	Vec r2 = Vec(1);        r2 << -1.000869225598735e-01;
	resection::M m2(R2, t2, r2, f2);
	solutions.push_back(m2);
	std::sort(solutions.begin(), solutions.end(), sortM);

	// PROCESS
	std::vector<resection::M> models;
	resection::P5PfrSolver::Solve(pt2D, pt3D, 1, &models);
	std::sort(models.begin(), models.end(), sortM);

	// TEST
	double eps = 1e-4;
	EXPECT_TRUE(testSolutions(solutions, models, eps));
}

TEST(Resection_P5Pfr, Test03) {
	// DATA
	Mat pt2D = Mat(2, 5);
	Mat pt3D = Mat(3, 5);
	pt2D << 7.426900000000000e+00, 1.109600000000000e+01, -6.599600000000000e+00, -7.966500000000000e+00, 9.185600000000000e-01, 9.001700000000000e+00, -8.493100000000000e+00, -8.015400000000000e+00, 6.563500000000000e+00, 4.434400000000000e-02;
	pt3D << 9.000000000000000e+00, 1.000000000000000e+01, -1.000000000000000e+01, -1.100000000000000e+01, 0.000000000000000e+00, 1.000000000000000e+01, -1.000000000000000e+01, -9.000000000000000e+00, 1.000000000000000e+01, 0.000000000000000e+00, 1.100000000000000e+01, 9.000000000000000e+00, 1.000000000000000e+01, 1.100000000000000e+01, 7.000000000000000e+00;

	// SOLUTIONS
	std::vector<resection::M> solutions;
	Mat R1 = Mat(3, 3);     R1 << 9.866726171702454e-01, -8.863862515507148e-02, -1.364563690599919e-01, 8.610372986162385e-02, 9.959878340728819e-01, -2.437995247586850e-02, 1.380698889343181e-01, 1.230562917641179e-02, 9.903460391500725e-01;
	Vec3 t1;                t1 << 2.338816295443713e+00, 2.374547827269040e-01, 3.038794646050726e+00;
	double                  f1 = 6.808663417966336e+00;
	Vec r1 = Vec(3);        r1 << -1.541773606559228e+00, 1.125710955774016e+00, -1.694590906260655e-01;
	resection::M m1(R1, t1, r1, f1);
	solutions.push_back(m1);
	Mat R2 = Mat(3, 3);     R2 << 9.911441225386224e-01, -9.612050154153130e-02, 9.161974427244664e-02, 9.612219384046608e-02, 9.953597965529948e-01, 4.404458794976624e-03, -9.161796880765205e-02, 4.441237370965423e-03, 9.957843256459575e-01;
	Vec3 t2;                t2 << -8.389662176239775e-04, 8.924922397161790e-05, 4.403363708227871e-03;
	double                  f2 = 1.001148139451480e+01;
	Vec r2 = Vec(3);        r2 << -1.013497349186711e-01, -9.446252525415940e-03, -1.094974551029621e-03;
	resection::M m2(R2, t2, r2, f2);
	solutions.push_back(m2);
	std::sort(solutions.begin(), solutions.end(), sortM);

	// PROCESS
	std::vector<resection::M> models;
	resection::P5PfrSolver::Solve(pt2D, pt3D, 3, &models);
	std::sort(models.begin(), models.end(), sortM);

	// TEST
	double eps = 1e-4;
	EXPECT_TRUE(testSolutions(solutions, models, eps));
}

TEST(Resection_P5Pfr, Test04) {
	// DATA
	Mat pt2D = Mat(2, 5);
	Mat pt3D = Mat(3, 5);
	pt2D << 7.442600000000000e+02, 1.116000000000000e+03, -6.606800000000000e+02, -7.975000000000000e+02, 9.185600000000000e+01, 9.020800000000000e+02, -8.541500000000000e+02, -8.024200000000000e+02, 6.570500000000000e+02, 4.434400000000000e+00;
	pt3D << 9.000000000000000e+00, 1.000000000000000e+01, -1.000000000000000e+01, -1.100000000000000e+01, 0.000000000000000e+00, 1.000000000000000e+01, -1.000000000000000e+01, -9.000000000000000e+00, 1.000000000000000e+01, 0.000000000000000e+00, 1.100000000000000e+01, 9.000000000000000e+00, 1.000000000000000e+01, 1.100000000000000e+01, 7.000000000000000e+00;

	// SOLUTIONS
	std::vector<resection::M> solutions;
	Mat R1 = Mat(3, 3);     R1 << -9.866929474192679e-01, 8.865005182074735e-02, 1.363018555459043e-01, -8.612834136352524e-02, -9.959876607489899e-01, 2.429996810126685e-02, 1.379091596923318e-01, 1.223715440509363e-02, 9.903692825027544e-01;
	Vec3 t1;                t1 << -2.337410845255324e+00, -2.368792577941530e-01, -8.313411561105022e+01;
	double                  f1 = 7.220370878892927e+03;
	Vec r1 = Vec(2);        r1 << -2.023313473211615e+01, 5.014947585259129e+02;
	resection::M m1(R1, t1, r1, f1);
	solutions.push_back(m1);
	Mat R2 = Mat(3, 3);     R2 << 9.911582426555702e-01, -9.612846516724592e-02, 9.145849441419568e-02, 9.612595944501501e-02, 9.953592630191308e-01, 4.442684187302039e-03, -9.146112800910124e-02, 4.388132473201126e-03, 9.957989788891634e-01;
	Vec3 t2;                t2 << 9.945786885438171e-04, -1.443041043924996e-04, -4.823891271638905e-03;
	double                  f2 = 9.988930531520911e+02;
	Vec r2 = Vec(2);        r2 << -9.900367868789792e-02, -1.021444916204432e-02;
	resection::M m2(R2, t2, r2, f2);
	solutions.push_back(m2);
	std::sort(solutions.begin(), solutions.end(), sortM);

	// PROCESS
	std::vector<resection::M> models;
	resection::P5PfrSolver::Solve(pt2D, pt3D, 2, &models);
	std::sort(models.begin(), models.end(), sortM);

	// TEST
	double eps = 1e-3;
	EXPECT_TRUE(testSolutions(solutions, models, eps));
}

TEST(Resection_P5Pfr, Test05) {
	// DATA
	Mat pt2D = Mat(2, 5);
	Mat pt3D = Mat(3, 5);
	pt2D << -5.083665639282905e+02, -1.888279680387158e+02, 5.485850494158182e+02, 6.057378448766012e+02, 1.049611103316406e+02, 6.135576285838145e+02, -1.221172634793433e+03, -3.523254600615324e+02, 3.014526831471967e+02, 6.551794931115911e+02;
	pt3D << -4.780000000000000e+00, -5.662000000000001e+01, -3.109000000000000e+01, -1.337000000000000e+01, -1.220000000000000e+00, -8.390000000000001e+00, -3.940000000000000e+00, 2.115000000000000e+01, 2.512000000000000e+01, 1.024000000000000e+01, 2.975400000000000e+02, 2.715000000000000e+02, 2.743600000000000e+02, 2.852800000000000e+02, 2.909600000000000e+02;

	// SOLUTIONS
	std::vector<resection::M> solutions;
	Mat R1 = Mat(3, 3);     R1 << 1.155567671802503e-01, 8.662877974144153e-01, -4.859959728328315e-01, 9.628483489629153e-01, 2.253109491131032e-02, 2.691011086218072e-01, 2.440690280601738e-01, -4.990368742016750e-01, -8.315001549782077e-01;
	Vec3 t1;                t1 << 1.361540537034142e+02, -5.564066632096895e+01, 3.181374378543369e+02;
	double                  f1 = 2.021855309782332e+03;
	Vec r1 = Vec(1);        r1 << 8.255860596348720e-01;
	resection::M m1(R1, t1, r1, f1);
	solutions.push_back(m1);
	Mat R2 = Mat(3, 3);     R2 << 2.922089312019317e-02, -9.842762550966916e-01, 1.742021614621672e-01, -7.850366300341083e-01, -1.304794743186365e-01, -6.055514811196674e-01, 6.187597505574750e-01, -1.190603226703449e-01, -7.765056410971429e-01;
	Vec3 t2;                t2 << -4.387772837075708e+01, 1.559300291237510e+02, -5.017373099470832e+01;
	double                  f2 = 8.380484450967588e+03;
	Vec r2 = Vec(1);        r2 << 8.284870911169731e+00;
	resection::M m2(R2, t2, r2, f2);
	solutions.push_back(m2);
	Mat R3 = Mat(3, 3);     R3 << -1.801610707703882e-02, 9.822319557442656e-01, -1.868041889267577e-01, 8.861462458888930e-01, 1.022150952868773e-01, 4.519921516934955e-01, 4.630553431100972e-01, -1.573926917308747e-01, -8.722426782764643e-01;
	Vec3 t3;                t3 << 4.770602230971817e+01, -1.100448483622631e+02, 3.787350883298001e+02;
	double                  f3 = 3.755300014149142e+03;
	Vec r3 = Vec(1);        r3 << -1.915149580218268e-02;
	resection::M m3(R3, t3, r3, f3);
	solutions.push_back(m3);
	Mat R4 = Mat(3, 3);     R4 << -1.174221990502533e-01, 9.889452839990088e-01, 9.054972350218801e-02, 9.756067174145514e-01, 9.784522777965984e-02, 1.965142344369749e-01, 1.854819670636651e-01, 1.114160520610315e-01, -9.763108640373349e-01;
	Vec3 t4;                t4 << -3.325815868705528e+01, -3.602692800212063e+01, 5.594982831629274e+02;
	double                  f4 = 9.255353840201478e+03;
	Vec r4 = Vec(1);        r4 << -1.274066589972153e+00;
	resection::M m4(R4, t4, r4, f4);
	solutions.push_back(m4);
	std::sort(solutions.begin(), solutions.end(), sortM);

	// PROCESS
	std::vector<resection::M> models;
	resection::P5PfrSolver::Solve(pt2D, pt3D, 1, &models);
	std::sort(models.begin(), models.end(), sortM);

	// TEST
	double eps = 1e-4;
	EXPECT_TRUE(testSolutions(solutions, models, eps));
}

TEST(Resection_P5Pfr, Test06) {
	// DATA
	Mat pt2D = Mat(2, 5);
	Mat pt3D = Mat(3, 5);
	pt2D << -5.023000000000000e+02, -1.816000000000000e+02, 5.608000000000000e+02, 6.180000000000000e+02, 1.138000000000000e+02, 6.223000000000000e+02, -1.226000000000000e+03, -3.493000000000000e+02, 3.088000000000000e+02, 6.635000000000000e+02;
	pt3D << -4.780000000000000e+00, -5.662000000000001e+01, -3.109000000000000e+01, -1.337000000000000e+01, -1.220000000000000e+00, -8.390000000000001e+00, -3.940000000000000e+00, 2.115000000000000e+01, 2.512000000000000e+01, 1.024000000000000e+01, 2.975400000000000e+02, 2.715000000000000e+02, 2.743600000000000e+02, 2.852800000000000e+02, 2.909600000000000e+02;

	// SOLUTIONS
	std::vector<resection::M> solutions;
	Mat R1 = Mat(3, 3);     R1 << 1.158093303645603e-01, 8.661302625338441e-01, -4.862165847886791e-01, 9.628499358654846e-01, 2.232902483922909e-02, 2.691122733239069e-01, 2.439430261441067e-01, -4.993193196470409e-01, -8.313675583175423e-01;
	Vec3 t1;                t1 << 1.364957077988834e+02, -5.546744398860090e+01, 3.201838786297712e+02;
	double                  f1 = 2.087740834197694e+03;
	Vec r1 = Vec(1);        r1 << 8.833511704539349e-01;
	resection::M m1(R1, t1, r1, f1);
	solutions.push_back(m1);
	Mat R2 = Mat(3, 3);     R2 << 3.005871612955928e-02, -9.845150899000348e-01, 1.727035359909268e-01, -7.819107066457760e-01, -1.307939675733752e-01, -6.095150407324811e-01, 6.226653358083665e-01, -1.167175042808154e-01, -7.737343883899128e-01;
	Vec3 t2;                t2 << -4.369759671728466e+01, 1.569393617329415e+02, -7.975480893440621e+01;
	double                  f2 = 9.270119409950667e+03;
	Vec r2 = Vec(1);        r2 << 1.030622373405317e+01;
	resection::M m2(R2, t2, r2, f2);
	solutions.push_back(m2);
	Mat R3 = Mat(3, 3);     R3 << -1.801610707683794e-02, 9.822319557441956e-01, -1.868041889271452e-01, 8.861462458893246e-01, 1.022150952867334e-01, 4.519921516926820e-01, 4.630553431092793e-01, -1.573926917314041e-01, -8.722426782768031e-01;
	Vec3 t3;                t3 << 4.797850960892938e+01, -1.098697561296521e+02, 3.782270371153628e+02;
	double                  f3 = 3.755281081447789e+03;
	Vec r3 = Vec(1);        r3 << 1.915149737466193e-02;
	resection::M m3(R3, t3, r3, f3);
	solutions.push_back(m3);
	Mat R4 = Mat(3, 3);     R4 << -1.199004189301619e-01, 9.879748719866909e-01, 9.761937237686719e-02, 9.764291491248300e-01, 9.957808492669369e-02, 1.914949653168269e-01, 1.794714636919536e-01, 1.182787272725487e-01, -9.766269177096513e-01;
	Vec3 t4;                t4 << -3.505229132105162e+01, -3.447366644241274e+01, 5.609300537327143e+02;
	double                  f4 = 9.400920059230813e+03;
	Vec r4 = Vec(1);        r4 << -1.441857759763210e+00;
	resection::M m4(R4, t4, r4, f4);
	solutions.push_back(m4);
	std::sort(solutions.begin(), solutions.end(), sortM);

	// PROCESS
	std::vector<resection::M> models;
	resection::P5PfrSolver::Solve(pt2D, pt3D, 1, &models);
	std::sort(models.begin(), models.end(), sortM);

	// TEST
	double eps = 1e-4;
	EXPECT_TRUE(testSolutions(solutions, models, eps));
}

TEST(Resection_P5Pfr, ReprojectionErrRD) {
	// DATA
	Mat R1 = Mat(3, 3);     R1 << -1.801610707703100e-02, 9.822319557442600e-01, -1.868041889267890e-01, 8.861462458886890e-01, 1.022150952869810e-01, 4.519921516938730e-01, 4.630553431104880e-01, -1.573926917308610e-01, -8.722426782762600e-01;
	Vec3 t1;                t1 << 4.770602230972729e+01, -1.100448483623751e+02, 3.787350878705134e+02;
	double                  f1 = 3.755300000000000e+03;
	Vec r1 = Vec(1);        r1 << -1.915149736941400e-02;
	resection::M m1(R1, t1, r1, f1);
	Mat X = Mat(3, 5);
	X << -4.780000000000000e+00, -5.662000000000001e+01, -3.109000000000000e+01, -1.337000000000000e+01, -1.220000000000000e+00, -8.390000000000001e+00, -3.940000000000000e+00, 2.115000000000000e+01, 2.512000000000000e+01, 1.024000000000000e+01, 2.975400000000000e+02, 2.715000000000000e+02, 2.743600000000000e+02, 2.852800000000000e+02, 2.909600000000000e+02;

	// SOLUTION
	Mat u = Mat(2, 5);
	u << -5.083665639282905e+02, -1.888279680387158e+02, 5.485850494158182e+02, 6.057378448766012e+02, 1.049611103316406e+02, 6.135576285838145e+02, -1.221172634793433e+03, -3.523254600615324e+02, 3.014526831471967e+02, 6.551794931115911e+02;

	// PROCESS & TEST
	double eps = 1e-4;
	for (int i = 0; i < 5; ++i){
		Mat ui = u.col(i);
		Mat Xi = X.col(i);
		EXPECT_TRUE( resection::reproj_error_RD(m1, Map<Vec2>(ui.data(),2), Map<Vec3>(Xi.data(),3)) < eps );
	}
}

TEST(Resection_P5Pfr, ReprojectionErrRP) {
	// DATA
	Mat R1 = Mat(3, 3);     R1 << -9.992060000000000e-01, -9.115130000000000e-04, -3.984260000000000e-02, -2.467660000000000e-03, 9.992350000000000e-01, 3.902560000000000e-02, 3.977660000000000e-02, 3.909300000000000e-02, -9.984440000000000e-01;
	Vec3 t1;                t1 << -9.757978898639005e-02, -1.432719535106094e+00, -7.429114914010337e-02;
	double                  f1 = 4.003890000000000e+03;
	Vec r1 = Vec(2);        r1 << 2.609980000000000e-01, -8.600540000000000e-01;
	resection::M m1(R1, t1, r1, f1);
	Mat X = Mat(3, 5);
	X << -1.789980000000000e+00, 3.126420000000000e-02, -8.818300000000000e-01, 4.003490000000000e-01, -2.801120000000000e-01, 5.379380000000000e-01, 2.120090000000000e+00, 7.626530000000000e-01, -2.713720000000000e-01, -5.288820000000000e-01, 7.695280000000000e+00, 8.668390000000001e+00, 8.591100000000001e+00, 8.153530000000000e+00, 6.836970000000000e+00;

	// SOLUTION
	Mat u = Mat(2, 5);
	u << -7.157065483965156e+02, 2.214390204167765e+02, -2.039687171099566e+02, 4.045162808436347e+02, 5.240453162355445e+01, 3.053701505359891e+02, -4.762317643178445e+02, 1.542630206562505e+02, 6.822162604030057e+02, 9.904720783975658e+02;

	// PROCESS & TEST
	double eps = 1e-3;
	for (int i = 0; i < 1; ++i) {
		Mat ui = u.col(i);
		Mat Xi = X.col(i);
		EXPECT_TRUE( resection::reproj_error_RP(m1, Map<Vec2>(ui.data(), 2), Map<Vec3>(Xi.data(), 3)) < eps);
	}
}

TEST(Resection_P5Pfr, ConversionRD2RP) {
	// DATA
	Mat pt2D = Mat(2, 5);
	pt2D << 4.811200000000001e+02, 3.067499999999998e+02, -6.962300000000000e+02, 1.242750000000000e+03, 6.765399999999997e+02, -1.153199999999999e+02, -9.486730000000000e+02, -9.567130000000000e+02, -8.622940000000000e+02, -1.491675000000000e+03;
	Mat R1 = Mat(3, 3);     R1 << -2.147969435011516e-01, -6.504746055378523e-01, -7.285225189470387e-01, -8.724048025820075e-01, 4.631236449953824e-01, -1.562893146636694e-01, 4.390582347416017e-01, 6.019960772268922e-01, -6.669547132369798e-01;
	Vec3 t1;                t1 << 5.835566436335433e-01, 5.556535697883296e-01, 8.118408025611844e-01;
	double                  f1 = 1.769320983964984e+03;
	Vec r1 = Vec(1);        r1 << -2.905907042217655e-01;
	resection::M m1(R1, t1, r1, f1);
	Mat R2 = Mat(3, 3);     R2 << -5.147789719583958e-01, -2.194669622313594e-01, -8.287562141657801e-01, -5.244123822170940e-01, -6.841333275746363e-01, 5.069055567648162e-01, -6.782287692267632e-01, 6.955543419392871e-01, 2.370862585696515e-01;
	Vec3 t2;                t2 << 1.273608583413997e+00, 2.506813631576317e-01, 2.141229474304712e+00;
	double                  f2 = 2.687625629669230e+03;
	Vec r2 = Vec(1);        r2 << -3.992521126906051e+00;
	resection::M m2(R2, t2, r2, f2);

	// SOLUTIONS ( EXPECTED VALUES )
	Vec e_r1 = Vec(3);		e_r1 << -2.767886578664726e-01, 1.084793023180692e-01, -2.249627924724677e-02;
	Vec e_r2 = Vec(3);		e_r2 << -2.058719740812550e+00, 9.689085352978840e-01, -1.271067176405148e-01;

	// PROCESS & TESTS ( ACTUAL VALUES )
	double eps = 1e-4;
	Mat pt2D_radius = pt2D.colwise().norm();
	
	rddiv2pol( &m1, pt2D_radius.maxCoeff(), (1 / f1) * pt2D_radius );
	EXPECT_TRUE(checkNear(m1._r, e_r1, eps));

	rddiv2pol(&m2, pt2D_radius.maxCoeff(), (1 / f2) * pt2D_radius);
	EXPECT_TRUE(checkNear(m2._r, e_r2, eps));
}


//TEST(Resection_P5Pfr, RandomRealExample) {
//
//
//	// PROCESS
//	std::vector<resection::M> models;
//	resection::P5PfrSolver::Solve(pt2D, pt3D, &models);
//	std::sort(models.begin(), models.end(), sortM);
//
//	// TEST
//	double eps = 1e-4;
//	EXPECT_TRUE(testSolutions(solutions, models, eps));
//}
/* ************************************************************************* */
int main() { TestResult tr; return TestRegistry::runAllTests(tr);}
/* ************************************************************************* */
