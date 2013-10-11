#pragma once

#include "glm/glm.hpp"
#include "glm/ext.hpp"

class GLTransform {
public:

	const static glm::vec3 FORWARD_VECTOR;
	const static glm::vec3 UP_VECTOR;
	const static glm::vec3 RIGHT_VECTOR;

	GLTransform() : orientation(glm::quat(1,0,0,0)), position(glm::vec3(0,0,0)) {}

	void Translate(float x, float y, float z) {
		this->position += glm::vec3(x, y, z);
		this->translateMatrix = glm::translate(this->translateMatrix, x, y, z);
		this->MMhasChanged = true;
	}

	void TranslateTo(float x, float y, float z) {
		this->position = glm::vec3(x, y, z);
		this->translateMatrix = glm::translate(x, y, z);
		this->MMhasChanged = true;
	}

	// Helper functions.
	void Translate(glm::vec3 trans) {
		Translate(trans.x, trans.y, trans.z);
	}

	void TranslateTo(glm::vec3 trans) {
		TranslateTo(trans.x, trans.y, trans.z);
	}

	void Rotate(float x, float y, float z) {
		glm::quat qX = glm::angleAxis(x, 1.0f,0.0f,0.0f);
		glm::quat qY = glm::angleAxis(y, 0.0f,1.0f,0.0f);
		glm::quat qZ = glm::angleAxis(z, 0.0f,0.0f,1.0f);
		glm::quat change = qX * qY * qZ;
		this->orientation = glm::normalize(change * this->orientation);
		this->rotateMatrix = glm::mat4_cast(this->orientation);
		this->MMhasChanged = true;
	}

	void Rotate(glm::vec3 rot) {
		Rotate(rot.x, rot.y, rot.z);
	}

	void Scale(float x, float y, float z) {
		this->scaleMatrix = glm::scale(this->scaleMatrix, x, y, z);
		this->MMhasChanged = true;
	}

	void Scale(glm::vec3 scale) {
		Scale(scale.x, scale.y, scale.z);
	}

	/*
	 * \brief Helper function to enable moving
	 * \the view based on global orientation vectors
	 *
	 * \param[in] float right
	 * \param[in] float up
	 * \param[in] float forward
	 * 
	 * \returns void
	*/
	void Move(float right, float up, float forward) {
		/*this->Translate(((FORWARD_VECTOR * forward) +
						(UP_VECTOR * up) +
						(RIGHT_VECTOR * right)) * this->orientation);*/
		/*this->position += (FORWARD_VECTOR * forward) * this->orientation;
		this->position += (UP_VECTOR * up) * this->orientation;
		this->position += (RIGHT_VECTOR * right) * this->orientation;

		this->translateMatrix = glm::translate(this->position);
		this->MMhasChanged = true;*/
		glm::vec3 forward_vector(this->rotateMatrix[2][0], this->rotateMatrix[2][1], this->rotateMatrix[2][2]);
		glm::vec3 right_vector(this->rotateMatrix[0][0], -this->rotateMatrix[0][1], this->rotateMatrix[0][2]);
		glm::vec3 up_vector(this->rotateMatrix[1][0], this->rotateMatrix[1][1], this->rotateMatrix[1][2]);

		this->Translate((forward_vector * forward) + (right_vector * right) + (up_vector * up));
	}

	glm::vec3 GetForward() {
		glm::vec3 forward_vector(this->rotateMatrix[2][0], this->rotateMatrix[2][1], this->rotateMatrix[2][2]);
		return forward_vector;
	}

	glm::vec3 GetUp() {
		glm::vec3 up_vector(this->rotateMatrix[1][0], this->rotateMatrix[1][1], this->rotateMatrix[1][2]);
		return up_vector;
	}

	// Convenience overload
	void Move(glm::vec3 vec) {
		this->Move(vec.x, vec.y, vec.z);
	}

	const glm::mat4 GetMatrix() {
		if (this->MMhasChanged) {
			this->transformMatrix = glm::mat4(1.0f);
			this->transformMatrix =  this->translateMatrix * this->rotateMatrix * this->scaleMatrix;
			//this->modelMatrix =  this->modelMatrix*this->scaleMatrix*this->rotateMatrix*this->translateMatrix;
			this->MMhasChanged = false;
		}

		return this->transformMatrix;
	}

	const glm::mat4 GetMatrixInverse() {
		return glm::inverse(this->GetMatrix());
	}

	const glm::vec3 GetPosition() {
		return this->position;
	}

	const glm::quat GetOrientation() {
		return this->orientation;
	}

private:
	glm::quat orientation;
	glm::vec3 position;

	glm::mat4 transformMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 rotateMatrix;
	glm::mat4 translateMatrix;
	bool MMhasChanged; // Set to true if the modelMatrix has changed and needs to be updated
};