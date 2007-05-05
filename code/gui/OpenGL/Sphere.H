#ifndef __Sphere_H__
#define __Sphere_H__

#include "Shape.H"

#define C_RADIUS   (0.5f)


class Sphere : public Shape
{
public:
   Sphere(int paramX, int paramY);
	virtual ~Sphere();
   
   virtual void getTextureCoord(Vertex *vert, float *u, float *v) const;
   
   // Methods required by pure virtual superclass Shape
   virtual int getShapeType() const;
   virtual void tessellate();
   
protected:
private:
};

#endif //__Sphere_H__

