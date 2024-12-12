#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "imgInfo.h"

namespace avm
{
	class IVehicleCamera
	{
	public:
		enum RectCorner {
			TOP_LEFT = 0,
			TOP_RIGHT,
			BOT_LEFT,
			BOT_RIGHT
		};

	private:
		camType m_camType;
		camPosition m_camPos;
		extrinsicParams m_exParams;
		cv::Mat m_srcImg;
		// undistortedImg should be a rectangular image. Each corner's world coordinates will be recorded in m_cornerWordCoords.
		cv::Mat m_undistortedImg;
		double m_cornerWordCoords[4][3];

	public:
		IVehicleCamera(camType t, camPosition p, extrinsicParams param);

		virtual ~IVehicleCamera();

		bool setExParams(extrinsicParams params);
		
		/**
		* This function helps set extrinsic param quaternion or translation.
		* 
		* @param type (const int) : 0 for quaternion, 1 for translation.
		* @param toset (vector<double>) : Param values to be set. 4 values for quaternion, 3 for translation.
		* @return (bool) : If the params are correctly set.
		*/
		bool setExparams(const int type, const std::vector<double>& toset);

		int getCameraType();
		int getCameraPositionType();
		extrinsicParams getExParams();

		void inputSourceImg(cv::Mat img);
		bool inputSourceImg(const char* filename);

		virtual bool dedistortSrc() = 0;
	};
}