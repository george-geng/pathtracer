#include "sphere.h"

#include <cmath>

#include  "../bsdf.h"
#include "../misc/sphere_drawing.h"

namespace CGL { namespace StaticScene {

bool Sphere::test(const Ray& r, double& t1, double& t2) const {

  // TODO Part 1, task 4:
  // Implement ray - sphere intersection test.
  // Return true if there are intersections and writing the
  // smaller of the two intersection times in t1 and the larger in t2.

  return false;

}

bool Sphere::intersect(const Ray& r) const {

  // TODO Part 1, task 4:
  // Implement ray - sphere intersection.
  // Note that you might want to use the the Sphere::test helper here.
  
  //return false;
  double a = dot(r.d, r.d);
  double b = 2*dot((r.o - o), r.d);
  double c = dot((r.o-o),(r.o-o)) - (this->r2);
  double disc = b*b - 4*a*c;
  if (disc >= 0) {
    double t = min((-b + sqrt(disc))/(2*a), (-b - sqrt(disc))/(2*a));
    if (t< r.min_t || t > r.max_t) {
      return false;
    } else {
      r.max_t = t;
      return true;
    }
  } else {
    return false;
  }

}

bool Sphere::intersect(const Ray& r, Intersection *i) const {

  // TODO Part 1m task 4:
  // Implement ray - sphere intersection.
  // Note again that you might want to use the the Sphere::test helper here.
  // When an intersection takes place, the Intersection data should be updated
  // correspondingly.
//UP UNTIL THIS POINT, ONLY RUN SPHERE_LAMBERTIAN
//  return false;
  double a = dot(r.d, r.d);
  double b = 2*dot((r.o - o), r.d);
  double c = dot((r.o-o),(r.o-o)) - (this->r2);
  double disc = b*b - 4*a*c;
  if (disc >= 0) {
    double t1 = min((-b + sqrt(disc))/(2*a), (-b - sqrt(disc))/(2*a));
    double t2 = max((-b + sqrt(disc))/(2*a), (-b - sqrt(disc))/(2*a));
    double t = t1;
    if (t < r.min_t || t > r.max_t) {
      t = t2;
      if (t< r.min_t || t > r.max_t)
        return false;
    } //else {
      r.max_t = t;
      Vector3D p = ((r.o + t * r.d)-o);
      p.normalize();
      i->n = p;
      i->t = t;
      i->primitive = this;
      i->bsdf = get_bsdf();
      return true;
    // }
  } else {
    return false;
  }

}

void Sphere::draw(const Color& c) const {
  Misc::draw_sphere_opengl(o, r, c);
}

void Sphere::drawOutline(const Color& c) const {
    //Misc::draw_sphere_opengl(o, r, c);
}


} // namespace StaticScene
} // namespace CGL
