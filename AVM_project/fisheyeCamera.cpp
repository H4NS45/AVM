#include "fisheyeCamera.h"
#include "quartic.h"

#include <cmath>

using namespace avm;
using namespace std;

cv::Vec3b fisheyeCamera::biInterpolateFromSrc(float u, float v)
{
	if (m_srcImg.empty())
	{
		std::cerr << "source image cannot be empty." << std::endl;
		assert(!m_srcImg.empty());
	}

	cv::Point2i p1, p2, p3, p4;
	cv::Vec3b I11, I12, I21, I22;
	int width = m_srcImg.size[1], height = m_srcImg.size[0];
	
	if (u<0 || u>width - 1 || v<0 || v>height - 1)
	{
		std::cerr << "Invalid (u,v)" << std::endl;
		assert(0);
	}

	p1 = cv::Point2i(floor(u), floor(v));
	p2 = cv::Point2i(ceil(u), floor(v));
	p3 = cv::Point2i(floor(u), ceil(v));
	p4 = cv::Point2i(ceil(u), ceil(v));

	I11 = m_srcImg.at<cv::Vec3b>(p1.y, p1.x);
	I12 = m_srcImg.at<cv::Vec3b>(p2.y, p2.x);
	I21 = m_srcImg.at<cv::Vec3b>(p3.y, p3.x);
	I22 = m_srcImg.at<cv::Vec3b>(p4.y, p4.x);

	// 计算在x方向上的插值
	float param1 = (p2.x - u) / (p2.x - p1.x), param2 = (u - p1.x) / (p2.x - p1.x),
		param3 = (p4.x - u) / (p4.x - p3.x), param4 = (u - p3.x) / (p4.x - p3.x),
		param5 = (p4.y - v) / (p4.y - p1.y), param6 = (v - p1.y) / (p4.y - p1.y);
	float f1 = param1 * I11[0] + param2 * I12[0];
	float f2 = param1 * I11[1] + param2 * I12[1];
	float f3 = param1 * I11[2] + param2 * I12[2];
	//float fa = param1 * I11[3] + param2 * I12[3];

	float f4 = param3 * I21[0] + param4 * I22[0];
	float f5 = param3 * I21[1] + param4 * I22[1];
	float f6 = param3 * I21[2] + param4 * I22[2];
	//float faa = param3 * I21[3] + param4 * I22[3];

	// 然后在y方向上的插值
	float b = param5 * f1 + param5 * f4;
	float g = param5 * f2 + param5 * f5;
	float r = param5 * f3 + param5 * f6;
	//float a = param5 * fa + param5 * faa;

	return cv::Vec3b(b, g, r);
}

fisheyeCamera::fisheyeCamera(camPosition p, extrinsicParams exparams, intrinsicParams inparams)
	:IVehicleCamera(camType::FISHEYE, p, exparams), m_inParams(inparams)
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

#if 1 // check the source image
	cv::imshow("source image", m_srcImg);
	cv::waitKey(0);
#endif

	int width = m_srcImg.size[1], height = m_srcImg.size[0];// source size.
	float cx_src = m_inParams.cx_offset, cy_src = m_inParams.cy_offset;// source camera offset
	int width_undistorted = width, height_undistorted = height;// the size of new camera image as well as the geometry size of the mapping surface.
	float focus = 0.2 * width_undistorted;// new camera's focus.
	cv::Mat K = (cv::Mat_<float>(3, 3) <<// new camera inParams.
		focus, 0, width_undistorted / 2 - 0.5,
		0, focus, height_undistorted / 2 - 0.5,
		0, 0, 1
		);

	m_undistortedImg = cv::Mat::zeros(height_undistorted, width_undistorted, CV_8UC3);
	
#if 0
	for (int i = 0; i < width_undistorted; i++)
	{
		for (int j = 0; j < height_undistorted; j++)
		{
			// get theta and Y_c/X_c from uv.
			double u_prime, v_prime;
			double rou, theta;
			double u_prime_new, v_prime_new, u_new, v_new;

			u_prime = i - cx_src - width / 2 + 0.5;
			v_prime = j - cy_src - height / 2 + 0.5;

			if (u_prime == 0 && v_prime == 0)
			{
				u_prime_new = 0;
				v_prime_new = 0;
			}
			else
			{
				rou = pow(u_prime * u_prime + v_prime * v_prime, 0.5);
				double k1 = m_inParams.k1, k2 = m_inParams.k2, k3 = m_inParams.k3, k4 = m_inParams.k4;
				std::complex<double>* theta_resolution = solve_quartic(k3 / k4, k2 / k4, k1 / k4, -1 * rou / k4);
				for (int i = 0; i < 4; i++)
				{
					if (theta_resolution[i].real() > 0 && theta_resolution[i].imag() == 0)
					{
						theta = theta_resolution[i].real();
						break;
					}
					if (i == 3)
					{
						std::cerr << "Quartic equation has no real root." << std::endl;
						assert(0);
					}
				}

				if (u_prime == 0)
				{
					u_prime_new = 0;
					
				}

			}

		}
	}
#else
	cv::Mat K_inv;
	if (!cv::invert(K, K_inv, cv::DECOMP_LU))
	{
		std::cerr << "failed to invert intrinsic matrix K!!!" << std::endl;
		__debugbreak();
	}

	for (int u = 0; u < width_undistorted; u++)
	{
		for (int v = 0; v < height_undistorted; v++)
		{
			// map each pixel coord of new image (u, v) to (Xc/Zc, Yc/Zc, 1). (Xc, Yc, Zc)s are coord of camera. 
			cv::Vec3f pixelCoord(u, v, 1);
			cv::Mat cameraCoord = K_inv * pixelCoord;
			float Xc_Zc = cameraCoord.at<float>(0, 0), Yc_Zc = cameraCoord.at<float>(1, 0);

			// get theta and (u_src, v_src)
			float chi_Zc = pow(Xc_Zc * Xc_Zc + Yc_Zc * Yc_Zc, 0.5);
			float theta = std::atan(chi_Zc);
			float rho = m_inParams.k1 * theta
				+ m_inParams.k2 * pow(theta, 2)
				+ m_inParams.k3 * pow(theta, 3)
				+ m_inParams.k4 * pow(theta, 4);
			float u_src_prime, v_src_prime, u_src, v_src;
			
			u_src_prime = chi_Zc == 0 ? 0 : (rho * Xc_Zc / chi_Zc);
			v_src_prime = chi_Zc == 0 ? 0 : (rho * Yc_Zc / chi_Zc);

			u_src = u_src_prime + m_inParams.cx_offset + width / 2 - 0.5;
			v_src = v_src_prime + m_inParams.cy_offset + height / 2 - 0.5;

			// biInterpolate the color.
			cv::Vec3b color = biInterpolateFromSrc(u_src, v_src);
			m_undistortedImg.at<cv::Vec3b>(v, u) = color;

		}
	}

#if 1
	cv::imshow("Undistorted image", m_undistortedImg);
	cv::waitKey(0);
#endif

#endif

	return true;
}
