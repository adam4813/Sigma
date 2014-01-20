#pragma once
#ifndef GLICOSPHERE_H
#define GLICOSPHERE_H

#include "resources/Mesh.h"

#include <map>
#include <stdint.h>

namespace Sigma{
	class GLIcoSphere : public Mesh {
	public:
		// We have a private ctor so the factory method must be used.
		GLIcoSphere();
		~GLIcoSphere(){}

		/**
		 * \brief Creates a new GLSprite
		 *
		 * This is the factory method to create a new GLIcoSphere. This method creates all the required buffers and fills them.
		 * \param entityID The entity this component belongs to
		 * \return   GLIcoSphere* The newly creates GLIcoSphere
		 */
		void InitializeBuffers();

		/**
		 * \brief Refines each equilateral triangle face into 4 sub-triangles.
		 *
		 * Recursively subdivides faces <level> times, so that the total number of faces at the end is
		 *  20 * 4^level. Works with the existing set of verts and faces.
		 * \param level The number of refinements to apply.
		 * \return   void
		 * \exception
		 */
		void Refine(int level);

		void RefineFace(const unsigned int index);

	private:
		// helper functions for refinement
		void RefineColor(const int v1, const int v2, float* green, float* blue) const;
		int CreateOrGetMidpoint(std::map<int64_t,int> &cache, const int v1, const int v2);
		/// given two vertices on the unit sphere, return their midpoint on the sphere
		Vertex GetUnitSphereMidPoint(const Vertex& v1, const Vertex& v2) const;
	}; // class GLIcoSphere

} // namespace Sigma

#endif // GLICOSPHERE_H
