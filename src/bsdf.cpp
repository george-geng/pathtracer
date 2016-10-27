#include "bsdf.h"

#include <iostream>
#include <algorithm>
#include <utility>

using std::min;
using std::max;
using std::swap;

namespace CGL {

void make_coord_space(Matrix3x3& o2w, const Vector3D& n) {

    Vector3D z = Vector3D(n.x, n.y, n.z);
    Vector3D h = z;
    if (fabs(h.x) <= fabs(h.y) && fabs(h.x) <= fabs(h.z)) h.x = 1.0;
    else if (fabs(h.y) <= fabs(h.x) && fabs(h.y) <= fabs(h.z)) h.y = 1.0;
    else h.z = 1.0;

    z.normalize();
    Vector3D y = cross(h, z);
    y.normalize();
    Vector3D x = cross(z, y);
    x.normalize();

    o2w[0] = x;
    o2w[1] = y;
    o2w[2] = z;
}

// Diffuse BSDF //

Spectrum DiffuseBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return albedo * (1.0 / PI);
}

Spectrum DiffuseBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *wi = sampler.get_sample(pdf);
  return albedo * (1.0 / PI);
}

// Mirror BSDF //

Spectrum MirrorBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum(); //mirror surface should always be 
  //impossible for any "outside source" besides sample_f() 
  //to sample/know the correct direction of reflection
}

Spectrum MirrorBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  // TODO Part 5:
  // Implement MirrorBSDF
  *pdf = 1.0f;
  Spectrum mirrorReflectance = reflectance;
  reflect(wo, wi); //this should give the correct value for wi
  //printf("mirror");
  //return Spectrum(0.5,0.5,0.5);
   return mirrorReflectance/(wi->z);
  //return Spectrum();
}

// Glossy BSDF //

/*
Spectrum GlossyBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum GlossyBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *pdf = 1.0f;
  return reflect(wo, wi, reflectance);
}
*/

// Refraction BSDF //

Spectrum RefractionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum RefractionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
//no need to implement this one
  // TODO Part 5:
  // Implement RefractionBSDF
  return Spectrum();
}

// Glass BSDF //

Spectrum GlassBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum GlassBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  // TODO Part 5:
  // Compute Fresnel coefficient and either reflect or refract based on it. 
  //first, test for total refraction
  //inputs 
  Vector3D dIn = wo;
  Vector3D normal = Vector3D(0.0, 0.0, 1.0);
  Spectrum lIn = transmittance;
  bool tir = !refract(wo, wi, ior);
  double ni;
  double no;
  bool entering = (wo.z > 0);
  if (entering) { //the glass from air
    no = ior;
    ni = 1.0;
  } else {
    no = 1.0;
    ni = ior;
  }
  // //outputs
  // Vector3D dOut;
//  double costheta = max(dot(normal, dIn), -dot(normal, dIn));
 // double costheta = abs_cos_theta(*wi);
  if (tir) {
    *pdf = 1.0;
    //reflect(wo, wi);
    Spectrum tirSpec = lIn/abs_cos_theta(*wi);
    return tirSpec;
  } else {
    double r0 = pow((ni-no)/(ni+no), 2);
    //dIn.normalize();
    double costheta = abs_cos_theta(wo); //here, wo or i?
  //  double costheta_i = cos_theta(wo); 
   // double costheta_t = sqrt(1.0-(ni/no)*(ni/no)*(1-costheta_i*costheta_i));
    //double r;
    //if (ni>no) //going from glass to air
    //{
    //  r = r0 + (1.0-r0)*pow(1-costheta_i, 5);  
    //} else {
    //  r = r0 + (1.0-r0)*pow(1-costheta_t, 5);
   // }
    double r = r0 + (1.0-r0)*pow(1-costheta, 5); 
    bool doReflect = coin_flip(r);
    if (doReflect) {
      *pdf = r;
      //reflect(wo, wi); //now set wi
      Spectrum reflSpec = r*lIn/abs_cos_theta(*wi);
      return reflSpec;
    } else { //do Refract
      *pdf = (1.0-r);
      //refract(wo, wi, ior);
      Spectrum refrSpec = (1.0-r)*lIn*(pow(no/ni,2))/abs_cos_theta(*wi);
      return refrSpec;
    }
  }
}

  // Vector3D normal = Vector3D(0.0f, 0.0f, 1.0f);
  // double ni;
  // double nt;
  // bool entering = (dot(wo, normal) > 0);
  // if (entering) {
  //   ni = 1.0;
  //   nt = ior; //glass's ior, = 1.5
  //   double cosIn = dot(input, normal);
  //   double sin_out_squared = ((nt*nt)/(ni*ni)) * (1-cosIn*cosIn);
  //   if (sin_out_squared > 1.0) {
  //     reflect(wo, wi); //now we set wi
  //   //then do total internal reflection
  //   } //no total internal reflection 
  //   else { //do refraction
  //     double r0 = pow((ni -nt)/(ni + nt), 2);
  //     double costheta = dot(normal, input);
  //     double r = r0 + (1.0-r0)*pow(1-costheta, 5); 
  //     bool doReflect = coin_flip(r);
  //     if (doReflect) {
  //       //return MirrorBSDF::sample_f(wo, wi, pdf);
  //     } else {
  //       //return a refraction spectrum
  //     }
  //   }
  // }
  
  //return Spectrum();
//}

void BSDF::reflect(const Vector3D& wo, Vector3D* wi) {
  // TODO Part 5:
  // Implement reflection of wo about normal (0,0,1) and store result in wi.
  Vector3D input = wo;
  input.normalize();
  Vector3D normal = Vector3D(0.0,0.0,1.0);
  Vector3D reflectVector = 2*dot(input, normal)*normal - input;
  reflectVector.normalize();
  *wi = reflectVector;
}

bool BSDF::refract(const Vector3D& wo, Vector3D* wi, float ior) {
  Vector3D input = wo;
  Vector3D normal = Vector3D(0.0, 0.0, 1.0);
  double ni;
  double no;
  bool entering = (wo.z > 0);
  if (entering) { //if entering
    no = ior;
    ni = 1.0;
  } else {
    no = 1.0;
    ni = ior;
  }
  if (entering) { //note: there can be no TIR here! we are good houston
    //double cos_in = dot(input, normal); 
    // double c = dot(-normal, -wo);
    double cos_in = cos_theta(wo);
    double n = ni/no;
    //Vector3D c = dot(-n, -wo);
    double sin_out_squared = (n*n)*sin_theta2(wo);
    double cos_out = sqrt(1.0 - sin_out_squared);
    Vector3D airToGlass = n*(-1.0*wo) + (n*cos_in-cos_out)*normal;
    airToGlass = Vector3D(-n*wo.x, -n*wo.y, -cos_out);
    // airToGlass.normalize();
 //   Vector3D airToGlass = n*(-wo) + (n*c-sqrt(1-n*n*(1-c*c)))*normal;
    *wi = airToGlass;
    return true;
  } else { //going from the glass to the air, normals still outwards facing
    double n = ni/no; //here, > 1
    normal = -normal;
    //double c = dot(-normal, -wo);
    double cos_in = cos_theta(wo);
    double sin_out_squared = (n*n)*sin_theta2(wo);
    //see if total internal reflection
    if (sin_out_squared > 1.0) {
      reflect(wo, wi); //sets wi to reflection angle
      return false;
    }
    double cos_out  = sqrt(1.0-sin_out_squared);
    // double length1 = sqrt(sin_out_squared);
    //Vector3D glassToAir = n*(-wo) + (n*c-sqrt(1-n*n*(1-c*c)))*normal;
    Vector3D glassToAir = n*(-1.0*wo) + (n*cos_in-cos_out) * normal;
    glassToAir = Vector3D(-n*wo.x, -n*wo.y, cos_out);
    //*wi = glassToAir;
    //Vector3D down = wo-dot((dot(normal, wo)), normal)*(no/ni);
    // Vector3D up = cos_in*normal;
    // Vector3D airToGlass = (down + up);
    // airToGlass.normalize();
    *wi = glassToAir;
    return true;
  }   
}

// Emission BSDF //

Spectrum EmissionBSDF::f(const Vector3D& wo, const Vector3D& wi) {
  return Spectrum();
}

Spectrum EmissionBSDF::sample_f(const Vector3D& wo, Vector3D* wi, float* pdf) {
  *pdf = 1.0 / PI;
  *wi  = sampler.get_sample(pdf);
  return Spectrum();
}

} // namespace CGL
