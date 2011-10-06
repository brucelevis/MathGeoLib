/* Copyright 2011 Jukka Jyl�nki

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License. */

/** @file Triangle.h
    @author Jukka Jyl�nki
	@brief The Triangle geometry object. */
#pragma once

#include "Math/MathFwd.h"
#include "Math/float3.h"

MATH_BEGIN_NAMESPACE

/// Specifies a triangle through three points in 3D space.
/** This class stores three member vertices a,b and c to specify the triangle. To determine which side
    of this triangle is the positive halfspace, the counter-clockwise (CCW) convention is used. This means that
    the positive halfspace is the one from which viewed the vertices a,b and c wind up in CCW order. */
class Triangle
{
public:
    float3 a;
    float3 b;
    float3 c;

    Triangle() {}
    /// Constructs a triangle given three points.
    /// The normal of the plane will be constructed to point towards the halfspace where
    /// the vertices a,b,c wind in counter-clockwise order.
    Triangle(const float3 &a, const float3 &b, const float3 &c);

    /// Expresses the given point in barycentric (u,v,w) coordinates with respect to this triangle.
    float3 BarycentricUVW(const float3 &point) const;

    float2 BarycentricUV(const float3 &point) const;

    /// Returns true if the given barycentric coordinates lie inside a triangle.
    /// That is, 0 <= u,v,w <= 1 and u+v+w==1.
    static bool BarycentricInsideTriangle(const float3 &barycentric);

    /// Returns a point at the given barycentric coordinates.
    /// The inputted u, v and w should sum up to 1.
    /// Returns u*a + v*b + w*c.
    float3 Point(float u, float v, float w) const;
    /// The same as above, except reads the u,v and w from the x,y and z components of the input vector.
    float3 Point(const float3 &barycentric) const;
    /// Returns a + (b-a)*u + (c-a)*v.
    float3 Point(float u, float v) const { return a + (b-a) * u + (c-a) * v; }

    /// Returns the center of mass of this Triangle.
    float3 Centroid() const;

    /// Returns the surface area of this triangle.
    float Area() const;

    /// Returns the given edge of this Triangle.
    /// @param i The edge of this triangle: 0, 1 or 2.
    LineSegment Edge(int i) const;

    /// Returns the given vertex of this Triangle.
    /// @param i The vertex of this triangle: 0, 1 or 2.
    float3 Vertex(int i) const;

    /// Returns the plane this triangle lies on. The normal of the returned plane points to the counter-clockwise direction of this triangle.
    Plane PlaneCCW() const;

    /// Returns the plane this triangle lies on. The normal of the returned plane points to the clockwise direction of this triangle.
    Plane PlaneCW() const;

    /// Returns the triangle normal when the positive side of this triangle is treated to be the counter-clockwise winding order. 
	/// That is, the resulting normal points to the direction where this plane is viewed to wind in CCW direction.
    float3 NormalCCW() const;

    /// Returns the triangle normal when the positive side of this triangle is treated to be the clockwise winding order. 
	/// That is, the resulting normal points to the direction where this plane is viewed to wind in CW direction.
	float3 NormalCW() const;

    /// Returns the unnormalized triangle normal.
    float3 UnnormalizedNormalCCW() const;
    float3 UnnormalizedNormalCW() const;

    /// Returns a Polygon representation of this Triangle.
    Polygon ToPolygon() const;

    /// Returns a Polyhedron representation of this Triangle.
    Polyhedron ToPolyhedron() const;

    /// Returns the surface area of the given 2D triangle.
    static float Area2D(const float2 &p1, const float2 &p2, const float2 &p3);

    /// Relates the barycentric coordinate of the given point to the surface area of triangle abc.
    static float SignedArea(const float3 &point, const float3 &a, const float3 &b, const float3 &c);

    /// Returns true if this triangle is degenerate.
    /// @param epsilon If two of the vertices of this triangle are closer than this, the triangle is considered
    /// degenerate.
    bool IsDegenerate(float epsilon = 1e-3f) const;

    /// Returns true if the given triangle defined by the three given points is degenerate (zero surface area).
    static bool IsDegenerate(const float3 &p1, const float3 &p2, const float3 &p3, float epsilon = 1e-3f);

    /// Returns true if the given point is contained in this triangle.
    /// @param triangleThickness The epsilon value to use for this test. This specifies the maximum distance
    /// the point lie from the plane defined by this triangle.
    bool Contains(const float3 &point, float triangleThickness = 1e-3f) const;
    bool Contains(const LineSegment &lineSegment, float triangleThickness = 1e-3f) const;
    bool Contains(const Triangle &triangle, float triangleThickness = 1e-3f) const;

    /// Returns the distance of the given point to this triangle.
    float Distance(const float3 &point) const;
    float Distance(const Sphere &sphere) const;

    bool Intersects(const LineSegment &lineSegment, float *d, float3 *intersectionPoint) const;
    bool Intersects(const Line &line, float *d, float3 *intersectionPoint) const;
    bool Intersects(const Ray &ray, float *d, float3 *intersectionPoint) const;
    bool Intersects(const Plane &plane) const;
    bool Intersects(const Sphere &sphere, float3 *closestPointOnTriangle) const;
    bool Intersects(const Sphere &sphere) const;
    /// @param outLine [out] Specifies the line segment of intersection if the two triangles intersect.
    bool Intersects(const Triangle &triangle, LineSegment *outLine) const;
    bool Intersects(const AABB &aabb) const;
    bool Intersects(const OBB &obb) const;
    bool Intersects(const Polygon &polygon) const;
    bool Intersects(const Frustum &frustum) const;
    bool Intersects(const Polyhedron &polyhedron) const;
    bool Intersects(const Capsule &capsule) const;

    /// Projects this Triangle to the given axis.
    /// @param axis The axis to project onto. This vector can be unnormalized.
    /// @param dMin [out] Returns the minimum extent of this triangle on the given axis.
    /// @param dMax [out] Returns the maximum extent of this triangle on the given axis.
    void ProjectToAxis(const float3 &axis, float &dMin, float &dMax) const;

    /// Returns the closest point on this triangle to the target object.
    float3 ClosestPoint(const float3 &targetPoint) const;
    float3 ClosestPoint(const LineSegment &other, float3 *otherPt) const;
//    float3 ClosestPoint(const Ray &other, float3 *otherPt) const;
    float3 ClosestPoint(const Line &other, float *outU, float *outV, float *outD) const;
    float3 ClosestPoint(const Triangle &other, float3 *otherPt) const;

    /// Returns the closest point on the edge of this triangle to the given object.
    float3 ClosestPointToTriangleEdge(const Line &line, float *outU, float *outV, float *outD) const;
    float3 ClosestPointToTriangleEdge(const LineSegment &lineSegment, float *outU, float *outV, float *outD) const;

    /// Generates a random point inside this Triangle.
    /** The points are distributed uniformly. */
    float3 RandomPointInside(LCG &rng) const;

#ifdef MATH_ENABLE_STL_SUPPORT
    /// Returns a human-readable representation of this Line. Most useful for debugging purposes.
    std::string ToString() const;
#endif
#ifdef QT_INTEROP
    operator QString() const { return toString(); }
    QString toString() const { return QString::fromStdString(ToString()); }
#endif
};

Triangle operator *(const float3x3 &transform, const Triangle &t);
Triangle operator *(const float3x4 &transform, const Triangle &t);
Triangle operator *(const float4x4 &transform, const Triangle &t);
Triangle operator *(const Quat &transform, const Triangle &t);

MATH_END_NAMESPACE

#ifdef QT_INTEROP
Q_DECLARE_METATYPE(Triangle)
Q_DECLARE_METATYPE(Triangle*)
#endif
