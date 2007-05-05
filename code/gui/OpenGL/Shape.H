#ifndef __Shape_H__
#define __Shape_H__

#define MAX_COORD    (.5)
#define MIN_COORD    (-MAX_COORD)
#define MIN_X        (MIN_COORD)
#define MIN_Y        (MIN_COORD)
#define MIN_Z        (MIN_COORD)
#define MAX_X        (MAX_COORD)
#define MAX_Y        (MAX_COORD)
#define MAX_Z        (MAX_COORD)

enum { V_X, V_Y, V_Z, V_W, N_DX = 0, N_DY, N_DZ };

typedef struct Vertex {
   float x;
   float y;
   float z;
   float w;
};

typedef struct Normal {
   float dx;
   float dy;
   float dz;
};

typedef struct Triangle {
   Vertex v0;
   Normal n0;
   Vertex v1;
   Normal n1;
   Vertex v2;
   Normal n2;
};

typedef struct TexCoord {
   float u;
   float v;
};


class Shape
{
public:
   Shape(int paramX, int paramY, const double *color);
	virtual ~Shape();
   
   virtual void getTextureCoord(Vertex *vert, float *u, float *v) const = 0;

   virtual void setColor(const double *color);
   virtual const double *getColor() const;
	
   // Methods for accessing/mutating Tessellation parameters
   virtual void setParameters(int paramX, int paramY);
   virtual int getParameterX() const;
   virtual int getParameterY() const;
   
   virtual unsigned int getNoTriangles() const;
   virtual float *getVertices() const;
   virtual float *getNormals() const;
   
   virtual void normalize(Normal *n) const;   
   virtual void allocateTriangles(unsigned int noTriangles);
   
   // Pure Virtual methods that all Shape subclasses must fill in
   virtual int getShapeType() const = 0;
   virtual void tessellate() = 0;
   
   virtual void draw() const;
   
protected:
   // Arrays to hold triangle Vertices and Normals
   Vertex *m_vertices;
   Normal *m_normals;
   TexCoord *m_textureCoords;
   
   unsigned int m_noTriangles;
   unsigned int m_allocatedTriangles;
   
   // Tessellation Parameters
   int m_paramX;
   int m_paramY;
   // Minimum Tessellation Parameters
   int m_minParamX;
   int m_minParamY;
   
   // Shape Properties needed for proper rendering by GL
   double m_color[3];
   
   bool m_cacheNormals;
   
private:
	void operator=(const Shape&);
};

#endif //__Shape_H__
