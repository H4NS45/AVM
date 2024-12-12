#pragma once
#include "IVehicleCamera.h"

namespace avm
{
	class fisheyeCamera
		:public IVehicleCamera
	{
	public:
		struct intrinsicParams
		{
			double aspect_ratio;
			double cx_offset;
			double cy_offset;
			double height;
			double k1;
			double k2;
			double k3;
			double k4;
			char* model;
			unsigned int poly_order;
			double width;
		};

	private:
		intrinsicParams m_inParams;

	public:
		fisheyeCamera(camPosition p, extrinsicParams outparam, intrinsicParams inparams);
		~fisheyeCamera();

		virtual bool dedistortSrc() override;

	};

}
