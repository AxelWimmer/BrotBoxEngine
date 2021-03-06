#include "BBE/Cube.h"
#include "BBE/VertexWithNormal.h"
#include "BBE/List.h"
#include "BBE/Math.h"
#include "string.h"


bbe::INTERNAL::vulkan::VulkanBuffer bbe::Cube::s_indexBuffer;
bbe::INTERNAL::vulkan::VulkanBuffer bbe::Cube::s_vertexBuffer;
size_t bbe::Cube::amountOfIndices = 0;

void bbe::Cube::s_init(VkDevice device, VkPhysicalDevice physicalDevice, INTERNAL::vulkan::VulkanCommandPool & commandPool, VkQueue queue)
{
	s_initVertexBuffer(device, physicalDevice, commandPool, queue);
	s_initIndexBuffer(device, physicalDevice, commandPool, queue);
}

void bbe::Cube::s_initIndexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, INTERNAL::vulkan::VulkanCommandPool & commandPool, VkQueue queue)
{
	bbe::List<uint32_t> indices = {
		 0,  1,  3,	//Bottom
		 1,  2,  3,
		 5,  4,  7,	//Top
		 6,  5,  7,
		 9,  8, 11,	//Left
		10,  9, 11,
		12, 13, 15,	//Right
		13, 14, 15,
		16, 17, 19,	//Front
		17, 18, 19,
		21, 20, 23,	//Back
		22, 21, 23,
	};

	amountOfIndices = indices.getLength();

	s_indexBuffer.create(device, physicalDevice, sizeof(uint32_t) * indices.getLength(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT);

	void* dataBuf = s_indexBuffer.map();
	memcpy(dataBuf, indices.getRaw(), sizeof(uint32_t) * indices.getLength());
	s_indexBuffer.unmap();

	s_indexBuffer.upload(commandPool, queue);
}

void bbe::Cube::s_initVertexBuffer(VkDevice device, VkPhysicalDevice physicalDevice, INTERNAL::vulkan::VulkanCommandPool & commandPool, VkQueue queue)
{
	bbe::List<INTERNAL::VertexWithNormal> vertices = {
		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5, -0.5), Vector3(0, 0, -1)),
		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5, -0.5), Vector3(0, 0, -1)),
		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5, -0.5), Vector3(0, 0, -1)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5, -0.5), Vector3(0, 0, -1)),

		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5,  0.5), Vector3(0, 0,  1)),
		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5,  0.5), Vector3(0, 0,  1)),
		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5,  0.5), Vector3(0, 0,  1)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5,  0.5), Vector3(0, 0,  1)),

		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5, -0.5), Vector3(0, -1, 0)),
		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5,  0.5), Vector3(0, -1, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5,  0.5), Vector3(0, -1, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5, -0.5), Vector3(0, -1, 0)),

		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5, -0.5), Vector3(0,  1, 0)),
		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5,  0.5), Vector3(0,  1, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5,  0.5), Vector3(0,  1, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5, -0.5), Vector3(0,  1, 0)),

		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5, -0.5), Vector3(-1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5,  0.5,  0.5), Vector3(-1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5,  0.5), Vector3(-1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3(-0.5, -0.5, -0.5), Vector3(-1, 0, 0)),

		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5, -0.5), Vector3( 1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3( 0.5,  0.5,  0.5), Vector3( 1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5,  0.5), Vector3( 1, 0, 0)),
		INTERNAL::VertexWithNormal(Vector3( 0.5, -0.5, -0.5), Vector3( 1, 0, 0)),
	};

	for (size_t i = 0; i < vertices.getLength(); i++)
	{
		vertices[i].m_normal = bbe::Math::interpolateLinear(vertices[i].m_pos, vertices[i].m_normal, 0.5f);

		vertices[i].m_normal = vertices[i].m_normal.normalize();
	}

	s_vertexBuffer.create(device, physicalDevice, sizeof(INTERNAL::VertexWithNormal) * vertices.getLength(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

	void* dataBuf = s_vertexBuffer.map();
	memcpy(dataBuf, vertices.getRaw(), sizeof(INTERNAL::VertexWithNormal) * vertices.getLength());
	s_vertexBuffer.unmap();

	s_vertexBuffer.upload(commandPool, queue);
}

void bbe::Cube::s_destroy()
{
	s_indexBuffer.destroy();
	s_vertexBuffer.destroy();
}

bbe::Cube::Cube()
{
	//UNTESTED
}

bbe::Cube::Cube(const Vector3 & pos, const Vector3 & scale, const Vector3 & rotationVector, float radians)
{
	//UNTESTED
	set(pos, scale, rotationVector, radians);
}

bbe::Cube::Cube(const Matrix4 & transform)
	: m_transform(transform)
{
	//UNTESTED
}


void bbe::Cube::set(const Vector3 & pos, const Vector3 & scale, const Vector3 & rotationVector, float radians)
{
	//UNTESTED
	const Matrix4 matTranslation = Matrix4::createTranslationMatrix(pos);
	const Matrix4 matScale = Matrix4::createScaleMatrix(scale);
	const Matrix4 matRotation = Matrix4::createRotationMatrix(radians, rotationVector);

	m_transform = matTranslation * matRotation * matScale;
}

void bbe::Cube::setRotation(const Vector3 & rotationVector, float radians)
{
	const Matrix4 matTranslation = Matrix4::createTranslationMatrix(m_transform.extractTranslation());
	const Matrix4 matScale = Matrix4::createScaleMatrix(m_transform.extractScale());
	const Matrix4 matRotation = Matrix4::createRotationMatrix(radians, rotationVector);

	m_transform = matTranslation * matRotation * matScale;
}

bbe::Vector3 bbe::Cube::getPos() const
{
	//UNTESTED
	return m_transform.extractTranslation();
}

float bbe::Cube::getX() const
{
	//UNTESTED
	return getPos().x;
}

float bbe::Cube::getY() const
{
	//UNTESTED
	return getPos().y;
}

float bbe::Cube::getZ() const
{
	//UNTESTED
	return getPos().z;
}

bbe::Vector3 bbe::Cube::getScale() const
{
	//UNTESTED
	return m_transform.extractScale();
}

float bbe::Cube::getWidth() const
{
	//UNTESTED
	return getScale().x;
}

float bbe::Cube::getHeight() const
{
	//UNTESTED
	return getScale().z;
}

float bbe::Cube::getDepth() const
{
	//UNTESTED
	return getScale().y;
}

bbe::Matrix4 bbe::Cube::getTransform() const
{
	//UNTESTED
	return m_transform;
}
