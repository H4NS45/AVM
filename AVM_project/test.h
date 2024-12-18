#pragma once
#include "fisheyeCamera.h"
#include "quartic.h"
#include <vector>
#include <iostream>

class test
{
public:
	test() {};
	~test() = default;

	void testQuartic(double a, double b, double c, double d, double e)
	{
		//double q[4] = { 0.5946970238045494, -0.5837953694518585, 0.39063952590941586, -0.3910488170060691 };
		//double t[3] = { 3.7484, 0.0, 0.6577999999999999 };
		//avm::camPosition position = avm::camPosition::FRONT;
		//avm::extrinsicParams exParams(q, t);
		//avm::fisheyeCamera::intrinsicParams inParams;
		//inParams.aspect_ratio = 1.0;
		//inParams.cx_offset = 3.942;
		//inParams.cy_offset = -3.093;
		//inParams.height = 966.0;
		//inParams.k1 = 339.749;
		//inParams.k2 = -31.988;
		//inParams.k3 = 48.275;
		//inParams.k4 = -7.201;
		//inParams.model = "radial_poly";
		//inParams.poly_order = 4;
		//inParams.width = 1280.0;

		//avm::fisheyeCamera* camera = new avm::fisheyeCamera(position, exParams, inParams);

		assert(a != 0);

		std::complex<double>* root = solve_quartic(b/a, c/a, d/a, e/a);

		std::cout << "real roots: ";

		for (int i=0; i<4; i++)
		{
			std::cout << root[i];
		}
	}

	void testDedistortion()
	{
		double q[4] = { 0.5946970238045494, -0.5837953694518585, 0.39063952590941586, -0.3910488170060691 };
		double t[3] = { 3.7484, 0.0, 0.6577999999999999 };
		avm::camPosition position = avm::camPosition::FRONT;
		avm::extrinsicParams exParams(q, t);
		avm::fisheyeCamera::intrinsicParams inParams;
		inParams.aspect_ratio = 1.0;
		inParams.cx_offset = 3.942;
		inParams.cy_offset = -3.093;
		inParams.height = 966.0;
		inParams.k1 = 339.749;
		inParams.k2 = -31.988;
		inParams.k3 = 48.275;
		inParams.k4 = -7.201;
		inParams.model = "radial_poly";
		inParams.poly_order = 4;
		inParams.width = 1280.0;

		avm::fisheyeCamera* camera = new avm::fisheyeCamera(position, exParams, inParams);

		camera->inputSourceImg("../res/rgb_images/00000_FV.png");

		camera->dedistortSrc();
	}
};