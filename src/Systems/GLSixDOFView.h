#pragma once

#include "IGLVIew.h"

struct GLSixDOFView : public IGLView {
	GLSixDOFView();

	void UpdateViewMatrix();

	void Move(float right, float up, float forward);

	void Rotate(float x, float y, float z);
};