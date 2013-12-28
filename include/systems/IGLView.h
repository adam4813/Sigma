#pragma once
#ifndef IGL_VIEW_H
#define IGL_VIEW_H

#include "GLTransform.h"
#include "components/SpatialComponent.h"

namespace Sigma{

	struct Plane {
		Plane() : distance(0), normal(0.0f, 1.0f, 0.0f) {}
		Plane(glm::vec4 vec) : normal(glm::vec3(vec.x, vec.y, vec.z)), distance(vec.w) {}

		void normalize() {
			float mag = glm::length(this->normal);
			this->normal /= mag;
			this->distance /= mag;
		}

		float distance;
		glm::vec3 normal;
	};

	struct Frustum {
		Plane planes[6];

		bool isectSphere(glm::vec3 position, float radius) {
			float distToPlane;

			// calculate our distances to each of the planes
			for(int i = 0; i < 6; ++i) {

				// find the distance to this plane
				distToPlane = (float)fabs(glm::dot(this->planes[i].normal, position) + this->planes[i].distance);

				// if this distance is < -sphere.radius, we are outside
				if(distToPlane < -radius)
					return false;

				// else if the distance is between +- radius, then we intersect
				if(distToPlane < radius)
					return true; // intersection
			}

			// otherwise we are fully in view
			return true;
		}
	};

    struct IGLView : public Sigma::SpatialComponent {

        IGLView(int entityID) : SpatialComponent(entityID) {}

		Frustum CameraFrustum;

        virtual const glm::mat4 GetViewMatrix() = 0;

		/*
		 * /brief Extracts frustum planes from a matrix
		 *        The space the planes are in depends on matrix
		 *        such that a Projection matrix only will yield camera space
		 *        view*proj will yield world space
		 */
		virtual void CalculateFrustum(glm::mat4 mvp) {
			this->CameraFrustum.planes[0] = Plane(mvp[3]+mvp[0]);
			this->CameraFrustum.planes[1] = Plane(mvp[3]-mvp[0]);
			this->CameraFrustum.planes[2] = Plane(mvp[3]-mvp[1]);
			this->CameraFrustum.planes[3] = Plane(mvp[3]+mvp[1]);
			this->CameraFrustum.planes[4] = Plane(mvp[3]+mvp[2]);
			this->CameraFrustum.planes[5] = Plane(mvp[3]-mvp[2]);

			this->CameraFrustum.planes[0].normalize();
			this->CameraFrustum.planes[1].normalize();
			this->CameraFrustum.planes[2].normalize();
			this->CameraFrustum.planes[3].normalize();
			this->CameraFrustum.planes[4].normalize();
			this->CameraFrustum.planes[5].normalize();
		}

    }; // stuct IGLView
} // namespace Sigma

#endif // IGL_VIEW_H
