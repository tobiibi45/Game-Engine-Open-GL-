#include "joint.h"
#include <glm/detail/func_integer.inl>


joint::joint(const int index, const std::string name, const glm::fmat4 bind_local_transform)
{
	this->index = index;
	this->name = name;
	this->local_bind_transform = bind_local_transform;
}

void joint::addchild(joint child)
{
	this->children.push_back(child);
}

glm::fmat4 joint::get_animated_transform() const
{
	return animated_transform;
}

void joint::setAnimationTransform(const glm::fmat4 animation_transform)
{
	this->animated_transform = animation_transform;
}

glm::fmat4 joint::get_inverse_bind_transform() const
{
	return inverse_bind_transform;
}

void joint::calc_inverse_bind_transform(glm::fmat4 parent_bind_transform)
{
	glm::fmat4 bind_transform = matrixCompMult(parent_bind_transform, local_bind_transform);
	this->inverse_bind_transform = inverse(bind_transform);
	for (auto child : children)
	{
		child.calc_inverse_bind_transform(bind_transform);
	}

}
