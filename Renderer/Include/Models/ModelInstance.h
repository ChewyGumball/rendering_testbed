#pragma once
#include <memory>
#include <Models/Model.h>
#include <glm/glm.hpp>

class ModelInstance
{
private:
	std::shared_ptr<Model> baseModel;
	std::vector<uint8_t> m_instanceData;

public:
	ModelInstance(std::shared_ptr<Model> model);
	~ModelInstance();

	void setState(std::string name, glm::vec2 value);
	void setState(std::string name, glm::vec3 value);
	void setState(std::string name, glm::vec4 value);
	void setState(std::string name, uint32_t value);
	void setState(std::string name, int32_t value);
	void setState(std::string name, bool value);
	void setState(std::string name, float value);
	void setState(std::string name, glm::mat3 value);
	void setState(std::string name, glm::mat4 value);

	glm::vec2 getVec2State(std::string name) const;
	glm::vec3 getVec3State(std::string name) const;
	glm::vec4 getVec4State(std::string name) const;
	uint32_t getUIntState(std::string name) const;
	int32_t getIntState(std::string name) const;
	bool getBoolState(std::string name) const;
	float getFloatState(std::string name) const;
	glm::mat3 getMat3State(std::string name) const;
	glm::mat4 getMat4State(std::string name) const;

	void rotate(glm::vec3 axis, float degrees);
	void translate(glm::vec3 offset);
	void scale(glm::vec3 scales);

	glm::vec3 position() const;

	std::shared_ptr<const Model> model() const;
	const std::vector<uint8_t>& instanceData() const;
	Culling::BoundingSphere bounds() const;
};

