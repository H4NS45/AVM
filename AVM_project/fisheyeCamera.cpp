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

}
