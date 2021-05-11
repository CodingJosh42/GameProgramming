#ifndef VECTOR2D
#define VECTOR2D

class Vector2D {
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& add(const Vector2D& other);
	Vector2D& subtract(const Vector2D& other);
	Vector2D& multiply(const Vector2D& other);
	Vector2D& divide(const Vector2D& other);

	friend Vector2D& operator+(Vector2D& vec1, const Vector2D& vec2);
	friend Vector2D& operator-(Vector2D& vec1, const Vector2D& vec2);
	friend Vector2D& operator*(Vector2D& vec1, const Vector2D& vec2);
	friend Vector2D& operator/(Vector2D& vec1, const Vector2D& vec2);

	Vector2D& operator+=(const Vector2D& other);
	Vector2D& operator-=(const Vector2D& other);
	Vector2D& operator*=(const Vector2D& other);
	Vector2D& operator/=(const Vector2D& other);
};

#endif
