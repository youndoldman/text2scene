#pragma once

#include <vector>
#include <sstream>
#include <QString>
#include "Math.h"
#include <QQuaternion>

#include <random>
#include <chrono>

#include <algorithm>

static std::vector<std::string> PartitionString(const std::string &s, const std::string &separator)
{
	std::vector<std::string> result;
	std::string curEntry;
	for (unsigned int outerCharacterIndex = 0; outerCharacterIndex < s.length(); outerCharacterIndex++)
	{
		bool isSeperator = true;
		for (unsigned int innerCharacterIndex = 0; innerCharacterIndex < separator.length() && outerCharacterIndex + innerCharacterIndex < s.length() && isSeperator; innerCharacterIndex++)
		{
			if (s[outerCharacterIndex + innerCharacterIndex] != separator[innerCharacterIndex]) {
				isSeperator = false;
			}
		}
		if (isSeperator) {
			if (curEntry.length() > 0) {
				result.push_back(curEntry);
				curEntry.clear();
			}
			outerCharacterIndex += separator.length() - 1;
		}
		else {
			curEntry.push_back(s[outerCharacterIndex]);
		}
	}
	if (curEntry.length() > 0) {
		result.push_back(curEntry);
	}
	return result;
}

static int StringToInt(const std::string &s)
{
	std::stringstream stream(std::stringstream::in | std::stringstream::out);
	stream << s;

	int result;
	stream >> result;
	return result;
}

static float StringToFloat(const std::string &s)
{
	std::stringstream stream(std::stringstream::in | std::stringstream::out);
	stream << s;

	float result;
	stream >> result;
	return result;
}

static std::vector<int> StringToIntegerList(const std::string &s, const std::string &prefix, const std::string &separator = " ")
{
	std::string subString;
	if (prefix == "")
	{
		subString = s;
	}
	else
	{
		subString = PartitionString(s, prefix)[0];
	}

	std::vector<std::string> parts = PartitionString(subString, separator);

	std::vector<int> result(parts.size());
	for (unsigned int resultIndex = 0; resultIndex < result.size(); resultIndex++)
	{
		result[resultIndex] = StringToInt(parts[resultIndex]);
	}
	return result;
}

static std::vector<float> StringToFloatList(const std::string &s, const std::string &prefix, const std::string &separator = " ")
{
	std::string subString;
	if (prefix == "")
	{
		subString = s;
	}
	else
	{
		subString = PartitionString(s, prefix)[0];
	}
	std::vector<std::string> parts = PartitionString(subString, separator);

	std::vector<float> result(parts.size());
	for (unsigned int resultIndex = 0; resultIndex < result.size(); resultIndex++)
	{
		result[resultIndex] = StringToFloat(parts[resultIndex]);
	}
	return result;
}

static vec3 TransformPoint(const mat4 &transMat, const vec3 &p)
{
	vec3 transP = vec3(transMat.a11*p.x + transMat.a12*p.y + transMat.a13*p.z + transMat.a14,
		transMat.a21*p.x + transMat.a22*p.y + transMat.a23*p.z + transMat.a24,
		transMat.a31*p.x + transMat.a32*p.y + transMat.a33*p.z + transMat.a34);

	return transP;
}

static vec3 TransformVector(const mat4 &transMat, const vec3 &v)
{
	vec3 transV = vec3(transMat.a11*v.x + transMat.a12*v.y + transMat.a13*v.z,
		transMat.a21*v.x + transMat.a22*v.y + transMat.a23*v.z,
		transMat.a31*v.x + transMat.a32*v.y + transMat.a33*v.z);

	return transV;
}

static mat4 GetRotationMatrix(const vec3 &fromVec, const vec3 &toVec)
{
	QQuaternion rotQ = QQuaternion::rotationTo(QVector3D(fromVec.x, fromVec.y, fromVec.z), QVector3D(toVec.x, toVec.y, toVec.z));
	QMatrix3x3 qrotMat = rotQ.toRotationMatrix();

	mat4 rotMat;
	rotMat.setToIdentity();
	
	rotMat.a11 = qrotMat(0, 0); rotMat.a12 = qrotMat(0, 1); rotMat.a13 = qrotMat(0, 2);
	rotMat.a21 = qrotMat(1, 0); rotMat.a22 = qrotMat(1, 1); rotMat.a23 = qrotMat(1, 2);
	rotMat.a31 = qrotMat(2, 0); rotMat.a32 = qrotMat(2, 1); rotMat.a33 = qrotMat(2, 2);

	return rotMat;
}

static mat4 GetTransformationMat(const mat4 &rotMat, const vec3 &currPos, const vec3 &tarPos)
{
	mat4 transMat;
	transMat.setToIdentity();

	vec3 transVec = tarPos - TransformPoint(rotMat, currPos);
	transMat = rotMat*transMat;
	transMat.a14 = transVec.x; transMat.a24 = transVec.y; transMat.a34 = transVec.z;

	return transMat;
}

static int GenRandomInt(int minV, int maxV)
{
	// generate a random number between minV and maxV, not including maxV

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::mt19937_64 rng(seed);
	//// initialize the random number generator with time-dependent seed
	//uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	//std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	//rng.seed(ss);
	// initialize a uniform distribution between 0 and 1
	std::uniform_int_distribution<int> unif(minV, maxV - 1);

	return unif(rng);
}

static double GenRandomDouble(double minV, double maxV)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937_64 rng(seed);
	//// initialize the random number generator with time-dependent seed
	//uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	//std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	//rng.seed(ss);
	// initialize a uniform distribution between 0 and 1
	std::uniform_real_distribution<double> unif(minV, maxV);

	return unif(rng);
};

static void GenNRandomDouble(double minV, double maxV, std::vector<double> &vals)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937_64 rng(seed);
	//// initialize the random number generator with time-dependent seed
	//uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	//std::seed_seq ss{ uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed >> 32) };
	//rng.seed(ss);
	// initialize a uniform distribution between 0 and 1
	std::uniform_real_distribution<double> unif(minV, maxV);


	for (int i = 0; i < vals.size(); i++)
	{
		vals[i] = unif(rng);
	}
};


// cannot put generator inside loop
static double GenNormalDistribution(double dMean, double stdDev)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0, 1);

	// using transformation rule of normal distribution
	return dMean + stdDev*distribution(generator);
}

static double GetNormalDistributionProb(double x, double dMean, double dVar)
{
	return (1.0 / sqrt(2 * math_pi))*exp(-0.5*(x - dMean)*(x - dMean) / dVar);
}

static void GenNNormalDistribution(const std::vector<double> &dMeans, const std::vector<double> &stdDevs, std::vector<double> &vals)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

	std::default_random_engine generator(seed);
	std::normal_distribution<double> distribution(0.0, 1.0);

	int gNum = dMeans.size();

	for (int i = 0; i < gNum; i++)
	{
		vals.push_back(dMeans[i] + stdDevs[i] * distribution(generator));
	}
}

static void EraseValueInVectorInt(std::vector<int> &v, int valueToErase)
{
	auto it = std::find(v.begin(), v.end(), valueToErase);
	if (it != v.end())
		v.erase(it);
}

static QString toQString(std::string s)
{
	return QString(s.c_str());
}

// math utility
//////////////////////////////////////////////
typedef struct
{
	uint x, y, z;
} uint3;

typedef struct
{
	float x, y, z;
} float3;

inline uint3 make_uint3(uint x, uint y, uint z) {
	uint3 t;
	t.x = x; t.y = y; t.z = z;
	return t;
}

inline float3 make_float3(float f) {
	float3 t;
	t.x = f; t.y = f; t.z = f;
	return t;
}

inline float3 make_float3(float x, float y, float z) {
	float3 t;
	t.x = x; t.y = y; t.z = z;
	return t;
}

inline float3 make_float3(float3 f) {
	float3 t;
	t.x = f.x; t.y = f.y; t.z = f.z;
	return t;
}

inline float3 normalize(float3 v) {
	float length = v.x * v.x + v.y * v.y + v.z * v.z;
	float oneOverLength = 1.0f / sqrtf(length);
	v.x *= oneOverLength;
	v.y *= oneOverLength;
	v.z *= oneOverLength;
	return v;
}

inline bool isnan3(float3 v) {
	return std::isnan(v.x) || std::isnan(v.y) || std::isnan(v.z);
}

/////////////////////
inline float3 make_float3(vec3 v)
{
	return make_float3(v.x, v.y, v.z);
}

inline float3 operator-(float3 f) {
	return make_float3(-f.x, -f.y, -f.z);
}

inline float3 operator-(float3 a, float3 b) {
	return make_float3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline float3 operator+(float3 a, float3 b) {
	return make_float3(a.x + b.x, a.y + b.y, a.z + b.z);
}

/////////////////////
inline float3 operator*(float3 a, float b) {
	return make_float3(a.x * b, a.y * b, a.z * b);
}

inline float3 operator*(float b, float3 a) {
	return make_float3(a.x * b, a.y * b, a.z * b);
}

inline float3 operator*(float3 a, float3 b) {
	return make_float3(a.x * b.x, a.y * b.y, a.z * b.z);
}

/////////////////
inline float3 operator/(float3 a, float b) {
	return make_float3(a.x / b, a.y / b, a.z / b);
}

inline float3 operator/(float3 a, float3 b) {
	return make_float3(a.x / b.x, a.y / b.y, a.z / b.z);
}

///////////
inline float3 float3Min(float3 a, float3 b) {
	return make_float3((std::min)(a.x, b.x), (std::min)(a.y, b.y), (std::min)(a.z, b.z));
}

inline float3 float3Max(float3 a, float3 b) {
	return make_float3((std::max)(a.x, b.x), (std::max)(a.y, b.y), (std::max)(a.z, b.z));
}


//////////////
inline float dot(float3 a, float3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float norm(float3 vec) {
	return sqrtf(dot(vec, vec));
}