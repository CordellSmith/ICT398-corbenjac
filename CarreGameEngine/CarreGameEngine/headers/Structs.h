/**
* @class Structs
* @brief File containing all structs
* @author Jack Matters
*
* @date 22/05/2017
* @version 1.0	Initial start.
* 
* @date 31/05/2018
* @version 2.0	Final version for submission.
*/

#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <vector>

/// Struct to hold all of a model types data (positions, scales, filePath to load)
struct ModelsData
{
	std::string filePath;
	std::string texFilePath;
	std::vector<std::vector<float>> modelPositions;
	std::vector<std::vector<float>> modelScales;
	std::vector<bool> isAI;
};

/// Struct to hold all of heightmaps data (positions, scales, size, filePath to load and texture)
struct HeightmapsData
{
	std::string filePath;
	std::string texFilePath;
	int fileSize;
	std::vector<float> modelPositions;
	std::vector<float> modelScales;
};



/// Contains all the operations required by Vector2 variables
/// Taken from old assignment
///TODO: Fix comments
struct Vector2
{
public:
	//x-axis value
	float x;
	//z-axis value
	float z;

	/**
	* @brief A contructor to define a 3D vector.
	* @param x - x-axis value
	* @param y - y-axis value
	* @param z - z-axis value
	*/
	Vector2(float x, float z) {
		Vector2::x = x;
		Vector2::z = z;
	};

	/**
	* @brief A default constructor to set the
	* vector's variables to 0.
	*/
	Vector2() { x = 0; z = 0; };

	/**
	* @brief A constructor to set all variables to the parameter's value.
	* @param num - the number to be set to.
	*/
	Vector2(float num) {
		x = num;
		z = num;
	}

	/**
	* @brief Overloads the equality operator to copy vectors.
	* @param A vector to be copied from.
	* @return a bew vector
	*/
	Vector2 operator = (const Vector2 original) {
		x = original.x;
		z = original.z;
		return Vector2(x, z);
	};

	/**
	* @brief Returns the dot product of two vectors.
	* @param one - the first vector
	* @param two - the second vector
	* @return a scalar value
	*/
	static float Dot(Vector2 one, Vector2 two) {
		return one.x*two.x + one.z *two.z;
	}

	/**
	* @brief Uses IO to display one vector.
	* @param os - reference to an output stream
	* @param vector - a vector to be displayed.
	*/
	friend std::ostream & operator <<(std::ostream & os, const Vector2 & vector) {
		os << vector.x << "  " << vector.z;
		return os;
	}

	/**
	* @brief The normal vector of a face.
	* @param a - the first point
	* @param b - the second point
	* @param c - the third point
	* @return Normal vector
	*/
	static Vector2 Normal(Vector2 a, Vector2 b, Vector2 c) {
		return Cross(a - b, c - b);
	}

	/**
	* @brief The cross product of two vectors.
	* @param first - the first vector
	* @param second - the second vector
	* @return the cross product
	*/
	static Vector2 Cross(const Vector2 first, const Vector2 second) {
		return Vector2(first.x * second.z + first.z * second.x);
	}

	/**
	* @brief Calculates the distance between to 3D points.
	* @param first - the first point
	* @param second - the second point
	* @return the distance between the points.
	*/
	static float Distance(const Vector2 first, const Vector2 second) {
		return sqrt((second.x - first.x)*(second.x - first.x) +
			(second.z - first.z)*(second.z - first.z));
	}

	/*
	* @brief Multiplies this vector by the parameter.
	* @param the scalar
	* @return the result of multiplication.
	*/
	Vector2 operator * (const float scalar) {
		x *= scalar;
		z *= scalar;
		return Vector2(x, z);
	};

	/*
	* @brief Adds two vectors together.
	* @param second - a vector which has to be added
	* @return the result of addition.
	*/
	Vector2 operator + (const Vector2 second) {
		this->x += second.x;
		this->z += second.z;
		return *this;
	}

	/*
	* @brief Subtracts the parameter from this vector.
	* @param second - a vector which has to be subtracted from this vector.
	* @return the result of subtraction.
	*/
	Vector2 operator - (const Vector2 second) {
		this->x -= second.x;
		this->z -= second.z;
		return *this;
	}

	/**
	* @brief An overlaoded array access operator to get one of the vector's values.
	* @param index - 0 - x value, 1 - y value, 2 - z value
	* @return the value of a particular variable or -1000000
	*/
	float operator[](const int index)  const {
		if (index == 0)
			return x;
		else if (index == 1)
			return z;
		else
			return -10000000;
	}

	/**
	* @brief Changes one of the variables of this vector.
	* @param index - 0 - x value, 1 - y value, 2 - z value
	* @param value - a new value to be assigned
	*/
	void Set(int index, float value) {
		if (index == 0)
			x = value;
		else if (index == 1)
			z = value;
	}

	/**
	* @brief Divides this vector by a scalar
	* @param a float to be divided by
	* @return the result of division
	*
	*/
	Vector2 operator / (const float copy) {
		x /= copy;
		z /= copy;
		return Vector2(x, z);
	}

	/*
	* @brief Calculates the length of this vector.
	* @return the length of the vector.
	*/
	float Length() {
		return sqrt((x*x) + (z*z));
	}

	/**
	* @brief Calculates a normalized version of the vector.
	* @return the normalized vector.
	*/
	Vector2 Normalized() const {
		float length = Vector2(x, z).Length();
		if (length == 0)
			return Vector2(0);
		else {
			return Vector2(x / length, z / length);
		}
	}
};
#endif