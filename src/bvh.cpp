#include "bvh.h"

#include "CGL/CGL.h"
#include "static_scene/triangle.h"

#include <iostream>
#include <stack>

using namespace std;

namespace CGL { namespace StaticScene {

BVHAccel::BVHAccel(const std::vector<Primitive *> &_primitives,
                   size_t max_leaf_size) {

  root = construct_bvh(_primitives, max_leaf_size);

}

BVHAccel::~BVHAccel() {
  if (root) delete root;
}

BBox BVHAccel::get_bbox() const {
  return root->bb;
}

void BVHAccel::draw(BVHNode *node, const Color& c) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->draw(c);
  } else {
    draw(node->l, c);
    draw(node->r, c);
  }
}

void BVHAccel::drawOutline(BVHNode *node, const Color& c) const {
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims))
      p->drawOutline(c);
  } else {
    drawOutline(node->l, c);
    drawOutline(node->r, c);
  }
}

BVHNode *BVHAccel::construct_bvh(const std::vector<Primitive*>& prims, size_t max_leaf_size) {
 
  // constructs a BVH from the given vector of primitives and maximum leaf
  // size configuration. first, build a BVH aggregate with a
  // single leaf node (which is also the root) that encloses all the
  // primitives.
  BBox centroid_box, bbox;
  for (Primitive *p : prims) {
    BBox bb = p->get_bbox();
    bbox.expand(bb);
    Vector3D c = bb.centroid();
    centroid_box.expand(c);
  }
 
  BVHNode *node = new BVHNode(bbox);
  if (prims.size() <=  max_leaf_size) { //leaf node
    node->prims = new vector<Primitive *>(prims);
    return node;
  } 
  //find the axis to recurse on--the max of extent dimenions
  double maxDim = max(max(bbox.extent.x, bbox.extent.y), bbox.extent.z);
  vector<Primitive *> left;
  vector<Primitive *> right;
  double split;
  if (bbox.extent.x == maxDim) {
    split = (bbox.max.x + bbox.min.x)*0.5; 
    for (Primitive *p : prims) {
      if (p->get_bbox().centroid().x <= split) {
        left.push_back(p);
      } else {
        right.push_back(p);
      }
    }
  } else if (bbox.extent.y == maxDim) {
    split = (bbox.max.y + bbox.min.y)*0.5; 
    for (Primitive *p : prims) {
      if (p->get_bbox().centroid().y <= split) {
        left.push_back(p);
      } else {
        right.push_back(p);
      }
    }
  } else {
    split = (bbox.max.z + bbox.min.z)*0.5; 
    for (Primitive *p : prims) {
      if (p->get_bbox().centroid().z <= split) {
        left.push_back(p);
      } else {
        right.push_back(p);
      }
    }
  }

  if (left.size() == 0 || right.size() ==0) {
    vector<Primitive *> leftNew;
    vector<Primitive *> rightNew;
    int half;
    if (left.size() == 0 && right.size() > 2) {
      half = (int)(right.size()*0.5);
      for (int i = 0; i < half; i++) {
      leftNew.push_back(right[i]);
      }
      for (int j = half; j < right.size(); j++) {
       rightNew.push_back(right[j]);   
      }   
    } else if (right.size() == 0 && left.size() > 2) {
      half = (int)(left.size()*0.5);
      for (int i = 0; i < half; i++) {
        leftNew.push_back(left[i]);
      }
      for (int j = half; j < left.size(); j++) {
         rightNew.push_back(left[j]);
       }
    } 
    node->l = construct_bvh(leftNew, max_leaf_size);
    node->r = construct_bvh(rightNew, max_leaf_size);
    // BVHNode *leftChild = new BVHNode(bbox);
  }
   
  else {//neither vector is empty
    node->l = construct_bvh(left, max_leaf_size);
    node->r = construct_bvh(right, max_leaf_size); 
  //  return;
  }
  return node;
 // */
}


bool BVHAccel::intersect(const Ray& ray, BVHNode *node) const {
  return false;

}

bool BVHAccel::intersect(const Ray& ray, Intersection* i, BVHNode *node) const {
  // BVH intersection.
  double tmin;
  double tmax;
  bool hit = false;
  bool bboxHit = node->bb.intersect(ray, tmin, tmax);
  if (!bboxHit) {
    return false;
  }  
  if (node->isLeaf()) {
    for (Primitive *p : *(node->prims)) {
      total_isects++;
      if (p->intersect(ray, i))      
        hit = true;
    } 
    return hit;
  } else {
    Intersection iL;//= Intersection();
    Intersection iR;//= Intersection();
    //Intersection iL = *i;
    //Intersection iR = *i;
    bool hitL = intersect(ray, &iL, node->l);
    bool hitR = intersect(ray, &iR, node->r);
  
    if (hitL && hitR) {
      if (iL.t < iR.t) {
        *i = iL;
        return true;
      } else {
        *i = iR;
        return true;
      }
    }
    else if (hitL) {
        *i = iL;
      return true;
    } else if (hitR) {
      *i = iR;
      return true;
    } 
   return false;
  }
}

}  // namespace StaticScene
}  // namespace CGL
