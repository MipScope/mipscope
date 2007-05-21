/*
 * MipScope <http://mipscope.sourceforge.net>
 * Copyright (C) 2007 Travis Fischer and Tim O'Donnell
 * 
 * Send bugs, features, ideas to
 * Travis Fischer (tfischer@cs.brown.edu) and Tim O'Donnell (tim@cs.brown.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 */

#include "Shape.H"

#include <Qt/QtOpenGL>
#include <QtGui>
using namespace QGL;

#include <iostream>
#include <math.h>

using namespace std;

Shape::Shape(int paramX, int paramY, const double *color = NULL) : m_vertices(NULL), m_normals(NULL), 
                                                                   m_textureCoords(NULL)
{
   m_paramX = paramX;
   m_paramY = paramY;

   m_noTriangles = 0;

   // Default minimum Parameter values which may be overridden
   // by subclasses
   m_minParamX = 1;
   m_minParamY = 1;
   
   m_cacheNormals = false;
   m_allocatedTriangles = 0;
   
   if (color)
      setColor(color);
   else m_color[0] = m_color[1] = m_color[2] = 1;
}


Shape::~Shape()
{
   if (m_vertices != NULL)
      delete[] m_vertices;

   if (m_normals != NULL)
      delete[] m_normals;
   
   if (m_textureCoords != NULL)
      delete[] m_textureCoords;

   m_vertices = NULL;
   m_normals  = NULL;
}

void Shape::setColor(const double *color) {
   if (color) {
      m_color[0] = color[0];
      m_color[1] = color[1];
      m_color[2] = color[2];
   }
}

const double *Shape::getColor() const {
   return m_color;
}

// Mutator for Parameters
void Shape::setParameters(int paramX, int paramY) {
   const int oldX = m_paramX, oldY = m_paramY; 

   if (paramX >= m_minParamX)
      m_paramX = paramX;
   else if (m_paramX > m_minParamX)
      m_paramX = m_minParamX;

   if (paramY >= m_minParamY)
      m_paramY = paramY;
   else if (m_paramY > m_minParamY)
      m_paramY = m_minParamY;

   if (m_paramX != oldX || m_paramY != oldY)
      tessellate();
}

// Accessor for curren X parameter
int Shape::getParameterX() const {
   return m_paramX;
}

// Accessor for curren Y parameter
int Shape::getParameterY() const {
   return m_paramY;
}

unsigned int Shape::getNoTriangles() const {
   return m_noTriangles;
}

// Accessor for Shape Vertices
float *Shape::getVertices() const {
   return (float*)m_vertices;
}

// Accessor for the Shape Normals
float *Shape::getNormals() const {
   return (float*)m_normals;
}

void Shape::normalize(Normal *n) const {
   float magnitude = sqrt(n->dx * n->dx + 
         n->dy * n->dy + n->dz * n->dz);

   if (magnitude != 0) {
      n->dx /= magnitude;
      n->dy /= magnitude;
      n->dz /= magnitude;
   }
}

void Shape::allocateTriangles(unsigned int noTriangles) {
   m_noTriangles = noTriangles;
   
   // Only reallocate memory if we have to
   if (noTriangles > m_allocatedTriangles) {
      if (m_vertices != NULL) {
         delete[] m_vertices;

         m_vertices = NULL;
      }
      if (m_normals != NULL) {
         delete[] m_normals;

         m_normals = NULL;
      }
      if (m_textureCoords != NULL) {
         delete[] m_textureCoords;

         m_textureCoords = NULL;
      }
      
      m_allocatedTriangles = noTriangles;
   }

   if (m_vertices == NULL)
      m_vertices = new Vertex[m_noTriangles * 3];
   if (m_normals == NULL)
      m_normals = new Normal[m_noTriangles * (m_cacheNormals ? 1 : 3)];
   if (m_textureCoords == NULL)
      m_textureCoords = new TexCoord[m_noTriangles * 3];
}

// Generalized draw method, which subclasses are able to specialize
void Shape::draw() const {
   unsigned int noTriangles = m_noTriangles;
   //long noVertices = m_noVertices;
   float *vertices = (float*)m_vertices;
   float  *normals = (float*)m_normals;
   float *tex      = (float*)m_textureCoords;

   // Color of Shape (defaults to white)
//   glColor3f(m_color[0], m_color[1], m_color[2]);
   
   glBegin(GL_TRIANGLES);
   if (m_cacheNormals) { 
      //float *oldNorms = normals;

      do {
         glNormal3fv(normals);
         normals  += 3;
         
         // Only update Normal if it differs from the last Normal set
         /*if (*oldNorms != *normals || 
               *(oldNorms + 1) != *(normals + 1) || 
               *(oldNorms + 2) != *(normals + 2)) {
            glNormal3fv(normals);
            oldNorms = normals;
         }*/
         
         glTexCoord2f(*tex, *(tex + 1)); tex += 2;
         glVertex4fv(vertices);
         glTexCoord2f(*tex, *(tex + 1)); tex += 2;
         glVertex4fv(vertices + 4);
         glTexCoord2f(*tex, *(tex + 1)); tex += 2;
         glVertex4fv(vertices + 8);

         vertices += 12;
      } while(--noTriangles);

   } else {
      do {
         glNormal3fv(normals);
         glTexCoord2f(*tex, *(tex + 1)); tex += 2;
         glVertex4fv(vertices);
         glNormal3fv(normals + 3);
         glTexCoord2f(*tex, *(tex + 1)); tex += 2;
         glVertex4fv(vertices + 4);
         glNormal3fv(normals + 6);
         glTexCoord2f(*tex, *(tex + 1)); tex += 2;
         glVertex4fv(vertices + 8);
         
         vertices += 12;
         normals  += 9;
      } while(--noTriangles);
   }
   
   glEnd();
   
   //std::cout << "(" << *vertices << ", " << *(vertices + 1) << ", " << 
   //   *(vertices + 2) << ", " << *(vertices + 3) << ")  " << endl;
   //std::cout << "(" << *normals  << ", " << *(normals + 1) << ", " << 
   //   *(normals + 2) << ")  " << endl;
}

