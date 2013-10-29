#pragma once

#ifndef CAMERAMOVER_H
#define CAMERAMOVER_H

#include "IMoverComponent.h"
#include "systems/IGLView.h"

namespace Sigma {
	class CameraMover : public IMoverComponent {
	public:
		CameraMover(const int entityID);

		void ApplyForces(const double delta);

		void RotateNow(float x, float y, float z);
		void RotateTarget(float x, float y, float z);

		void View(IGLView* view);

		IGLView* View();

		IGLView* view;
	};
};
#endif