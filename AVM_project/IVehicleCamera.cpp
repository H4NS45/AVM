#include "IVehicleCamera.h"

using namespace avm;

IVehicleCamera::IVehicleCamera(camType t, camPosition p, extrinsicParams param)
	: m_camType(t), m_camPos(p), m_exParams(param)
{
	// initialize coords memory.
	memset(m_cornerWordCoords, 0, sizeof(double) * 12);

	return;
}

IVehicleCamera::~IVehicleCamera()
{

}

int IVehicleCamera::getCameraType()
{
	return m_camType;
}

int IVehicleCamera::getCameraPositionType()
{
	return m_camPos;
}

extrinsicParams IVehicleCamera::getExParams()
{
	return m_exParams;
}

bool IVehicleCamera::setExParams(extrinsicParams params)
{
	m_exParams = params;
	return true;
}

bool IVehicleCamera::setExparams(const int type, const std::vector<double>& toset)
{
	switch (type)
	{
	case 0:
		// quaternion
		if (toset.size() != 4)
			return false;
		
		for (int i = 0; i < 4; i++)
		{
			m_exParams.quaternion[i] = toset[i];
		}
		break;
	case 1:
		// translation
		if (toset.size() != 3)
			return false;
		
		for (int i = 0; i < 3; i++)
		{
			m_exParams.translation[i] = toset[i];
		}
		break;
	}

	return true;
}

void IVehicleCamera::inputSourceImg(cv::Mat img)
{
	m_srcImg = img;
}

bool IVehicleCamera::inputSourceImg(const char* filename)
{
	cv::Mat img = cv::imread(filename);
	if (img.empty())
		return false;

	m_srcImg = img;
}