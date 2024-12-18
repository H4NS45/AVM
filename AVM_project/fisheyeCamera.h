#pragma once
#include "IVehicleCamera.h"
#include <string>

class test;

namespace avm
{
	class fisheyeCamera
		:public IVehicleCamera
	{
		friend test;
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
			std::string model;
			unsigned int poly_order;
			double width;
		};

	private:
		intrinsicParams m_inParams;
		
		/**
		 * @params u,v (float) : point in the source image domain within [0, width/height - 1].
		 * @return (cv::Vec4b) : BGRA color.
		 */
		cv::Vec3b biInterpolateFromSrc(float u, float v);

	public:
		fisheyeCamera(camPosition p, extrinsicParams exparam, intrinsicParams inparams);
		~fisheyeCamera();

		virtual bool dedistortSrc() override;

	};

}
