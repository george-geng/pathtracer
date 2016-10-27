#include "bbox.h"

#include "GL/glew.h"

#include <algorithm>
#include <iostream>

namespace CGL {

bool BBox::intersect(const Ray& r, double& t0, double& t1) const {
  // TODO Part 2, task 2:
  // Implement ray - bounding box intersection test
  // If the ray intersected the bounding box within the range given by
  // t0, t1, update t0 and t1 with the new intersection times.
  double tminX = (double)(min.x - r.o.x) / (double)r.d.x;
  double  tmaxX = (double)(max.x - r.o.x) / (double)r.d.x;
  if (tminX > tmaxX) {
    double a = tminX;
    tminX = tmaxX;
    tmaxX = a;
  }
  double tminY = (double)(min.y - r.o.y) / (double)r.d.y;
  double tmaxY = (double)(max.y - r.o.y) / (double)r.d.y;
  if (tminY > tmaxY) {
    double b = tminY;
    tminY = tmaxY;
    tmaxY = b;
  }
  if ((tminX > tmaxY) || (tminY > tmaxX)) {
    return false;
  }
  double tminZ = (double)(min.z - r.o.z) / (double)r.d.z;
  double tmaxZ = (double)(max.z - r.o.z) / (double)r.d.z;
  if (tminZ > tmaxZ) {
    double c = tminZ;
    tminZ = tmaxZ;
    tmaxZ = c;
  }

  double tminXY =  std::max(tminX, tminY);
  double tmaxXY = std::min(tmaxX, tmaxY);
  if ((tminXY > tmaxZ) || (tminZ > tmaxXY)) 
     return false;

  double tminXYZ = std::max(tminXY, tminZ);
  double tmaxXYZ = std::min(tmaxXY, tmaxZ);

  if(tminXYZ > r.max_t || tmaxXYZ < r.min_t)
      return false;
  else {
    t0 = tminXYZ;
    t1 = tmaxXYZ;
    return true;
  }
  // if (tminY > tminX) {
  //   tminX = tminY;
  // } 
  // if (tmaxY < tmaxX) {
  //   tmaxX = tmaxY;
  // }

  // if (tminZ > tmaxZ) {
  //   double c = tminZ;
  //   tminZ = tmaxZ;
  //   tmaxZ = c;
  // }
  // if ((tminX > tmaxZ) || (tminZ > tmaxX)) 
  //   return false;

  // if (tminZ > tminX) 
  //   tminX = tminZ;
  // if (tmaxZ < tmaxX) 
  //   tmaxX = tmaxZ;
  
  // t0 = tminX;
  // t1 = tmaxX;
  // return true;
}

void BBox::draw(Color c) const {

  glColor4f(c.r, c.g, c.b, c.a);

	// top
	glBegin(GL_LINE_STRIP);
	glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, max.y, min.z);
  glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, max.y, max.z);
  glVertex3d(max.x, max.y, max.z);
	glEnd();

	// bottom
	glBegin(GL_LINE_STRIP);
  glVertex3d(min.x, min.y, min.z);
  glVertex3d(min.x, min.y, max.z);
  glVertex3d(max.x, min.y, max.z);
  glVertex3d(max.x, min.y, min.z);
  glVertex3d(min.x, min.y, min.z);
	glEnd();

	// side
	glBegin(GL_LINES);
	glVertex3d(max.x, max.y, max.z);
  glVertex3d(max.x, min.y, max.z);
	glVertex3d(max.x, max.y, min.z);
  glVertex3d(max.x, min.y, min.z);
	glVertex3d(min.x, max.y, min.z);
  glVertex3d(min.x, min.y, min.z);
	glVertex3d(min.x, max.y, max.z);
  glVertex3d(min.x, min.y, max.z);
	glEnd();

}

std::ostream& operator<<(std::ostream& os, const BBox& b) {
  return os << "BBOX(" << b.min << ", " << b.max << ")";
}

} // namespace CGL
