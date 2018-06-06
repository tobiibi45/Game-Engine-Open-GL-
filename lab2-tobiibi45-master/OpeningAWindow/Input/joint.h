#pragma once
#include <string>
#include <vector>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <functional>

class joint 
{

public:
	joint(int index, std::string name, glm::fmat4 bind_local_transform);

	int index;
	std::string name;
	std::vector<joint> children;

	glm::fmat4 animated_transform;

	glm::fmat4 local_bind_transform;
	glm::fmat4 inverse_bind_transform;

	void addchild(joint child); // Adds a child joint to this joint. Used during creation of joints
	glm::fmat4 get_animated_transform() const; //This is the transform that is loaded back to the shader
	void setAnimationTransform(glm::fmat4 animation_transform); //This method allows those all important "joint tansforms"
	glm::fmat4 get_inverse_bind_transform() const; //returns the inverted model-space bind tranform.
	void calc_inverse_bind_transform(glm::fmat4 parent_bind_transform); //This is called during set-up, after the joints hierachy is known


};
