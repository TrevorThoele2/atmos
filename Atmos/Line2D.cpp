#include "Line2D.h"
#include "Math.h"

namespace Atmos
{
    Line2D::Line2D(const Position2D& first, const Position2D& second) : first(first), second(second)
    {}

    bool Line2D::operator==(const Line2D& arg) const
    {
        return first == arg.first && second == arg.second;
    }

    bool Line2D::operator!=(const Line2D& arg) const
    {
        return !(*this == arg);
    }

    Line2D::operator Vector2D() const
    {
        return Vector2D(second.x - first.x, second.y - first.y);
    }

    Optional<Position2D::Value> Line2D::Slope() const
    {
        typedef Optional<Position2D::Value> RetT;
        const Position2D::Value x = second.x - first.x;
        const Position2D::Value y = second.y - first.y;
        return (x == 0) ? RetT() : (y / x);
    }

    Optional<Position2D::Value> Line2D::YIntercept() const
    {
        typedef Optional<Position2D::Value> RetT;
        const RetT slope(Slope());
        return (!slope.IsValid()) ? RetT() : (-1 * *slope * first.x) + first.y;
    }

    Position2D::Value Line2D::Length() const
    {
        return std::sqrt(std::pow(second.x - first.x, 2) + std::pow(second.y - first.y, 2));
    }

    ::Atmos::Angle Line2D::Angle() const
    {
        return ::Atmos::Angle(Radians(std::atan2(DeltaY(), DeltaX())));
    }

    ::Atmos::Angle Line2D::Angle(const Line2D& other) const
    {
        return other.Angle() - Angle();
    }

    Position2D::Value Line2D::Dot(const Line2D& other) const
    {
        return (first.x * second.x) + (first.y * second.y);
    }

    Position2D::Value Line2D::Cross(const Line2D& other) const
    {
        return first.x * second.y - second.x * first.y;
    }

    Vector2D Line2D::Direction() const
    {
        return Vector2D(second.x - first.x, second.y - first.y);
    }

    bool Line2D::CheckIntersect(const Line2D& other) const
    {
        // If the points coincide, then they intersect
        if (first == other.first || first == other.second || second == other.first || second == other.second)
            return true;

        const Vector2D direction(Direction());
        const Vector2D otherDirection(other.Direction());
        // Parallelity
        if (direction == otherDirection)
            return false;



        /*
        if (first.GetX() == second.GetX())
            return !(other.first.GetY() == other.second.GetY() && first.GetX() != other.first.GetX());
        else if (other.first.GetX() == other.second.GetX())
            return true;
        else
            return (first.GetY() - second.GetY()) / (first.GetX() - second.GetX()) != (other.first.GetY() - other.second.GetY() / other.first.GetX() - other.second.GetX());
        */
        
        return false;
    }

    Optional<Position2D> Line2D::IntersectionPoint(const Line2D& other, bool infinite) const
    {
        /*
        Two lines
        (x1,y1),(x2,y2) and (x3,y3),(x4,y4)


        Call the slopes m1=(y2-y1)/(x2-x1) and m3=(y4-y3)/(x4-x3)

        Call the y-intercepts of these lines b1=y1-x1m1 and b3=y3-x3m3

        If the lines are parallel, m1=m3.
        Such lines could intersect, but likely at multiple points.  This would happen if and only if b1=b3 and the range of the two segments overlaps. (that is, some of the x values between x1 and x2 were also in the range of x3 to x4).

        If one line is vertical, then m would be 0/0.  In this case, the other line would intersect if and only if the x-value of the vertical line (that is, x1=x2) exists in the range of the non-vertical line. (that is, if x1=x2 is between x3 and x4).

        If both lines are vertical, then the lines would intersect if and only if their x-values were identical (that is, x1=x2=x3=x4) and the range of y-values of one line overlapped the other (that is, if some of the y values between y1 and y2 were also between y3 and y4).

        For all other cases, the following should work:

        The x-point of intersection of these segments (if they were turned into infinite lines) would be (b1-b3)/(m3-m1).

        Now that you have the x-value, make sure that x is between x1 and x2, and then make sure x is between x3 and x4.  If it satisfies both of these, then the segments intersect.

        */

        /*
        // Check for vertical case
        if (first.GetX() == second.GetX())
        {
            // If the X value is between the other two, then we have an intersection
            if((first.GetX() >= other.first.GetX() && first.GetX() <= other.first.GetX()) || )
        }

        Position2D::ValueT m1 = (second.GetY() - first.GetY()) / (second.GetX() - first.GetX());
        Position2D::ValueT m2 = (other.second.GetY() - other.first.GetY()) / (other.second.GetX() - other.first.GetX());
        if (m1 == m2)
            ;

        Position2D::ValueT b1 = first.GetY() - (first.GetX() * m1);
        Position2D::ValueT b2 = other.first.GetY() - (other.first.GetX() * m2);
        */

        return Optional<Position2D>();
    }

    Position2D::Value Line2D::DeltaX() const
    {
        return second.x - first.x;
    }

    Position2D::Value Line2D::DeltaY() const
    {
        return second.y - first.y;
    }
}