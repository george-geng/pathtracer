#include "triangle.h"

#include "CGL/CGL.h"
#include "GL/glew.h"

namespace CGL { namespace StaticScene {

Triangle::Triangle(const Mesh* mesh, size_t v1, size_t v2, size_t v3) :
    mesh(mesh), v1(v1), v2(v2), v3(v3) { }

BBox Triangle::get_bbox() const {

  Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  BBox bb(p1);
  bb.expand(p2); 
  bb.expand(p3);
  return bb;

}

bool Triangle::intersect(const Ray& r) const {
  
  //Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  //return false;
  Vector3D p0(mesh->positions[v1]), p1(mesh->positions[v2]), p2(mesh->positions[v3]); //3 vectord 3Ds!
  //not every intersection is valid -- the ray has min_t and max_t fields defining the valid range of t values. 
  If t lies outside this range, you should return false. Else, update max_t to be equal to t so that future intersections with 
  farther away primitives will be discarded.*/
  Vector3D e1 = p1-p0;
  Vector3D e2 = p2-p0;
  Vector3D s = r.o - p0;
  Vector3D s1 = cross(r.d, e2);
  Vector3D s2 = cross(s, e1);

  double det = dot(e1, s1);
  double invDet = 1.0/det;
  double u = dot(s1, s) *invDet;
  if (u < 0 || u > 1) {
    return false;
  }
//  Vector3D qvec = cross(tvec, v0v1);
  double v = dot(r.d,s2) * invDet;
   if (v < 0 || u+v > 1) {
    return false;
  }
  double t = dot(e2, s2) * invDet;
  if (t > r.max_t || t < r.min_t) {
    return false;
  } else {
    r.max_t = t;
    return true;
  }
}

bool Triangle::intersect(const Ray& r, Intersection *isect) const {
    
  // ray-triangle intersection. 
  // intersection data should be updated accordingly
  //Vector3D p1(mesh->positions[v1]), p2(mesh->positions[v2]), p3(mesh->positions[v3]);
  //Vector3D n1(mesh->normals[v1]), n2(mesh->normals[v2]), n3(mesh->normals[v3]);
  Vector3D p0(mesh->positions[v1]), p1(mesh->positions[v2]), p2(mesh->positions[v3]);
  Vector3D n1(mesh->normals[v1]), n2(mesh->normals[v2]), n3(mesh->normals[v3]);
  
  Vector3D e1 = p1-p0;
  Vector3D e2 = p2-p0;
  Vector3D s = r.o - p0;
  Vector3D s1 = cross(r.d, e2);
  Vector3D s2 = cross(s, e1);

  double det = dot(e1, s1);
  double invDet = 1.0/det;
  double u = dot(s1, s) *invDet;
  if (u < 0 || u > 1) {
    return false;
  }
//  Vector3D qvec = cross(tvec, v0v1);
  double v = dot(r.d,s2) * invDet;
   if (v < 0 || u+v > 1) {
    return false;
  }
  double t = dot(e2, s2) * invDet;
  if (t > r.max_t || t < r.min_t) {
    return false;    
  } else {
   // printf("u and v are coo");
    r.max_t = t;
    isect->t = t;
    Vector3D p = (1-u-v)*n1 + u*n2 + v*n3;
  //  Vector3D p = u*n1 + v*n2 + (1-u-v)*n3;
    //p.normalize();
    isect->n = p;
    isect->primitive = this;
    isect->bsdf = get_bsdf();
    return true;
  }
  //return false;
}

void Triangle::draw(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_TRIANGLES);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}

void Triangle::drawOutline(const Color& c) const {
  glColor4f(c.r, c.g, c.b, c.a);
  glBegin(GL_LINE_LOOP);
  glVertex3d(mesh->positions[v1].x,
             mesh->positions[v1].y,
             mesh->positions[v1].z);
  glVertex3d(mesh->positions[v2].x,
             mesh->positions[v2].y,
             mesh->positions[v2].z);
  glVertex3d(mesh->positions[v3].x,
             mesh->positions[v3].y,
             mesh->positions[v3].z);
  glEnd();
}



} // namespace StaticScene
} // namespace CGL
