
#ifndef meshmodel_H_
#define meshmodel_H_

#include "AGLM.h"

namespace agl {
   class Mesh
   {
   private:
       float* m_Positions;
       float* m_Normals;
       glm::vec3 m_MaxBounds;
       glm::vec3 m_MinBounds;
       glm::vec3 m_AvgPos = { 0, 0, 0 };
       unsigned int m_NumVertices;
       unsigned int* m_Indices;
       unsigned int m_NumTriangles;
   public:

      Mesh();

      virtual ~Mesh();

      // Initialize this object with the given file
      // Returns true if successfull. false otherwise.
      bool loadPLY(const std::string& filename);

      // Return the minimum point of the axis-aligned bounding box
      glm::vec3 getMinBounds() const;

      // Return the maximum point of the axis-aligned bounding box
      glm::vec3 getMaxBounds() const;

      glm::vec3 getAvgPosition() const;

      // Return number of vertices in this model
      int numVertices() const;

      // Positions in this model
      float* positions() const;

      // Normals in this model
      float* normals() const;

      // Return number of faces in this model
      int numTriangles() const;

      // face indices in this model
      unsigned int* indices() const;

   };
}

#endif
