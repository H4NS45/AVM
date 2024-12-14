#include "fisheyeCamera.h"

using namespace avm;

fisheyeCamera::fisheyeCamera(camPosition p, extrinsicParams outparams, intrinsicParams inparams)
	:IVehicleCamera(camType::FISHEYE, p, outparams), m_inParams(inparams)
{
	return;
}

fisheyeCamera::~fisheyeCamera()
{

}

bool fisheyeCamera::dedistortSrc()
{
	if (m_srcImg.empty())
	{
		std::cerr << "Source image should not be empty!!!\n";
		return false;
	}

	cv::Mat K = (cv::Mat_<double>(3, 3) <<
		500, 0, 320,    // f_x = 500, c_x = 320
		0, 500, 240,    // f_y = 500, c_y = 240
		0, 0, 1
		);

	return true;
}
