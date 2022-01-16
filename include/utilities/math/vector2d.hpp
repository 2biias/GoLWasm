#if !defined(VECTOR2D_HPP)
#define VECTOR2D_HPP

namespace Utilities::Math {

class Vector2D
{
public:
    Vector2D()
    : x_(0), y_(0)
    {}

    Vector2D(int x, int y)
    : x_(x), y_(y)
    {}

    Vector2D(std::pair<int, int> pos)
    : Vector2D(pos.first, pos.second)
    {}

    Vector2D(const Vector2D& other) 
    : Vector2D(other.x_, other.y_)
    {}

    Vector2D(Vector2D&& other) noexcept
    : x_(std::move(other.x_)), y_(std::move(other.y_))
    {}

    Vector2D& operator=(const Vector2D& rhs) {
        if(&rhs != this) {
            x_ = rhs.x_;
            y_ = rhs.y_;
        }
        return *this;
    }

    Vector2D& operator=(Vector2D&& rhs) {
        if(&rhs != this) {
            x_ = rhs.x_;
            y_ = rhs.y_;
        }
        return *this;
    }

    ~Vector2D() = default;

    int getX() const { return x_; }
    int getY() const { return y_; }

    Vector2D operator-(const Vector2D& vec) const {
        int x_n, y_n;
        x_n = x_ - vec.getX();
        y_n = y_ - vec.getY();
        return Vector2D(x_n, y_n);
    }

    Vector2D operator+(const int& integer) const {
        int x_n, y_n;
        x_n = x_ + integer;
        y_n = y_ + integer;
        return Vector2D(x_n, y_n);
    }

private:
    int x_;
    int y_;
};

}

#endif // VECTOR2D_HPP