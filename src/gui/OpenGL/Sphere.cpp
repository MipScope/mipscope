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

#include "Sphere.H"

#include <QtGui>
#include <QtOpenGL/QtOpenGL>
using namespace QGL;

#include <iostream>
#include <math.h>

using namespace std;

Sphere::Sphere(int paramX, int paramY) : Shape(paramX, paramY, NULL)
{
   // Minimum Parameters
   m_minParamX = 2;
   m_minParamY = 3;
   
   m_paramX = (paramX < m_minParamX) ? m_minParamX : paramX;
   m_paramY = (paramY < m_minParamY) ? m_minParamY : paramY;
   
   tessellate();
}

Sphere::~Sphere()
{ }

int Sphere::getShapeType() const {
   return 4;
}

void Sphere::getTextureCoord(Vertex *vert, float *u, float *v) const {
   *u = 1 - atan2(vert->z, vert->x) * (1 / (2 * M_PI));
   *v = 0.5f - asin(vert->y * 2) * (1 / M_PI);
}

void Sphere::tessellate() {
   // Number of Vertices = 3 vertices per triangle * 
   // 2 triangles per square patch * 
   // number of latitude divisions (m_paramX) * 
   // number of longitude divisions (m_paramY)
   
   //cout << "Tessellating: paramX = " << m_paramX << "; paramY = " << m_paramY << endl;
   allocateTriangles(2 * m_paramX * m_paramY);
   
   float *vertex = (float*)m_vertices;
   float *normal = (float*)m_normals;
   float *tex    = (float*)m_textureCoords;
   
   const float latAdd = (M_PI) / (float)(m_paramX);
   const float longAdd = (2 * M_PI) / (float)(m_paramY);
   float theta = 0;
   int a, b;
   
   // Massive Loop Unrolling for Speed
   for(b = m_paramY; b--; theta += longAdd) {
      const float cosTheta  = C_RADIUS * cos(theta);
      const float cos2Theta = C_RADIUS * cos(theta + longAdd);
      const float sinTheta  = C_RADIUS * sin(theta);
      const float sin2Theta = C_RADIUS * sin(theta + longAdd);
      
      register float phi = 0;
      for(a = m_paramX; a--; phi += latAdd) {
         // 
         // First Triangle within square patch
         // 
         vertex[V_X] = sin(phi) * cosTheta;
         vertex[V_Y] = C_RADIUS * cos(phi);
         vertex[V_Z] = sin(phi) * sinTheta;
         vertex[V_W] = 1;
         
         getTextureCoord((Vertex*)vertex, tex, tex + 1);tex += 2;
         
         normal[N_DX] = vertex[V_X] / C_RADIUS;
         normal[N_DY] = vertex[V_Y] / C_RADIUS;
         normal[N_DZ] = vertex[V_Z] / C_RADIUS;
         normal += 3;
         vertex += 4;
                 // Third Vertex
         vertex[V_X] = sin(phi) * cos2Theta;
         vertex[V_Y] = C_RADIUS * cos(phi);
         vertex[V_Z] = sin(phi) * sin2Theta;
         vertex[V_W] = 1;
         getTextureCoord((Vertex*)vertex, tex, tex + 1);tex +=2;
         
         normal[N_DX] = vertex[V_X] / C_RADIUS;
         normal[N_DY] = vertex[V_Y] / C_RADIUS;
         normal[N_DZ] = vertex[V_Z] / C_RADIUS;
         normal += 3;
         vertex += 4;
 
         // Second Vertex
         vertex[V_X] = sin(phi + latAdd) * cosTheta;
         vertex[V_Y] = C_RADIUS * cos(phi + latAdd);
         vertex[V_Z] = sin(phi + latAdd) * sinTheta;
         vertex[V_W] = 1;
         getTextureCoord((Vertex*)vertex, tex, tex + 1);tex +=2;
         
         normal[N_DX] = vertex[V_X] / C_RADIUS;
         normal[N_DY] = vertex[V_Y] / C_RADIUS;
         normal[N_DZ] = vertex[V_Z] / C_RADIUS;
         normal += 3;
         vertex += 4;
         

         // 
         // Second Triangle
         //
         vertex[V_X] = sin(phi + latAdd) * cosTheta;
         vertex[V_Y] = C_RADIUS * cos(phi + latAdd);
         vertex[V_Z] = sin(phi + latAdd) * sinTheta;
         vertex[V_W] = 1;
         getTextureCoord((Vertex*)vertex, tex, tex + 1);tex +=2;
         
         normal[N_DX] = vertex[V_X] / C_RADIUS;
         normal[N_DY] = vertex[V_Y] / C_RADIUS;
         normal[N_DZ] = vertex[V_Z] / C_RADIUS;
         normal += 3;
         vertex += 4;
        // Third Vertex
         vertex[V_X] = sin(phi) * cos2Theta;
         vertex[V_Y] = C_RADIUS * cos(phi);
         vertex[V_Z] = sin(phi) * sin2Theta;
         vertex[V_W] = 1;
         getTextureCoord((Vertex*)vertex, tex, tex + 1);tex +=2;
         
         normal[N_DX] = vertex[V_X] / C_RADIUS;
         normal[N_DY] = vertex[V_Y] / C_RADIUS;
         normal[N_DZ] = vertex[V_Z] / C_RADIUS;
         normal += 3; 
         vertex += 4;
 
         // Second Vertex
         vertex[V_X] = sin(phi + latAdd) * cos2Theta;
         vertex[V_Y] = C_RADIUS * cos(phi + latAdd);
         vertex[V_Z] = sin(phi + latAdd) * sin2Theta;
         vertex[V_W] = 1;
         getTextureCoord((Vertex*)vertex, tex, tex + 1);tex +=2;
         
         normal[N_DX] = vertex[V_X] / C_RADIUS;
         normal[N_DY] = vertex[V_Y] / C_RADIUS;
         normal[N_DZ] = vertex[V_Z] / C_RADIUS;
         normal += 3;
         vertex += 4;
      }
   }
}

