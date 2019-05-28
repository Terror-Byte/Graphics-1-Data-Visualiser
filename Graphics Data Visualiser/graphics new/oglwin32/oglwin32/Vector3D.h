#pragma once
#include <string>

class Vector3D
{
	public:
						Vector3D(void);
						Vector3D(float x, float y, float z);
						Vector3D(const Vector3D& v);
						~Vector3D(void);

		float			GetX() const;
		float			GetY() const;
		float			GetZ() const;

		void			SetX(float xIn);
		void			SetY(float yIn);
		void			SetZ(float yIn);

		/*Vector3D& operator= (const Vector3D &rhs);
		const Vector3D operator+(const Vector3D &other) const;
		const Vector3D operator-(const Vector3D &other) const;*/

	private:
		float x;
		float y;
		float z;

		void Init(float xIn, float yIn, float zIn);
		void Copy(const Vector3D& v);
};