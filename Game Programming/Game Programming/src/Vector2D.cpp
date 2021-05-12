#include "../include/Vector2D.h"

Vector2D::Vector2D() {
	x = 0.0f;
	y = 0.0f;
}
Vector2D::Vector2D(float x, float y) {
	this->x = x;
	this->y = y;
}

Vector2D& Vector2D::add(const Vector2D& other) {
	this->x += other.x;
	this->y += other.y;

	return *this;
}

Vector2D& Vector2D::subtract(const Vector2D& other) {
	this->x -= other.x;
	this->y -= other.y;

	return *this;
}

Vector2D& Vector2D::multiply(const Vector2D& other) {
	this->x *= other.x;
	this->y *= other.y;

	return *this;
}

Vector2D& Vector2D::divide(const Vector2D& other) {
	this->x /= other.x;
	this->y /= other.y;

	return *this;
}

Vector2D& operator+(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.add(vec2);
}
Vector2D& operator-(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.subtract(vec2);
}
Vector2D& operator*(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.multiply(vec2);
}
Vector2D& operator/(Vector2D& vec1, const Vector2D& vec2) {
	return vec1.divide(vec2);
}



Vector2D& Vector2D::operator+=(const Vector2D& other) {
	return add(other);
}
Vector2D& Vector2D::operator-=(const Vector2D& other) {
	return subtract(other);
}
Vector2D& Vector2D::operator*=(const Vector2D& other) {
	return multiply(other);
}
Vector2D& Vector2D::operator/=(const Vector2D& other) {
	return divide(other);
}