#pragma once


namespace avm
{
	enum varType
	{
		AVM_INT = 0,
		AVM_FLOAT,
		AVM_DOUBLE,
		AVM_STR,
		AVM_CHAR,
	};

	enum camType
	{
		NONE_CAM_TYPE = 0,
		FISHEYE
	};

	enum camPosition {
		NONE_CAM_POS = 0,
		FRONT,
		REAR,
		MVL,
		MVR
	};

	struct extrinsicParams
	{
		double quaternion[4];
		double translation[3];

		extrinsicParams(double q[4], double t[3])
		{
			for (int i = 0; i < 4; i++)
			{
				quaternion[i] = q[i];
			}

			for (int i = 0; i < 3; i++)
			{
				translation[i] = t[i];
			}
		}
	};

}