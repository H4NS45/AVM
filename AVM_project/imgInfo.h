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
		NONE = 0,
		FISHEYE
	};

	enum camPosition {
		NONE = 0,
		FRONT,
		REAR,
		MVL,
		MVR
	};

	struct extrinsicParams
	{
		double quaternion[4];
		double translation[3];
	};

}