#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include "imgInfo.h"
#include "IVehicleCamera.h"

namespace avm
{

	class mapConstructor
	{
	private:
		const int m_camCount;
		std::vector<IVehicleCamera> m_cameras;
		cv::Mat m_fusionMap;

	public:
		mapConstructor();
		~mapConstructor();

		bool setCamSource(const int pos, cv::Mat src);
		bool setCamSource(const int pos, const char* filename);

		bool dedistort(const int pos);
		bool dedistort(std::vector<bool>& bitmap);
		bool dedistort();

		bool fusion();

	};

}