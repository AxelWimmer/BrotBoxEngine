#pragma once

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"
#include "../BBE/VulkanHelper.h"
#include "../BBE/ColorByte.h"
#include "../BBE/Color.h"
#include "../BBE/String.h"
#include "../BBE/VulkanDescriptorSet.h"

namespace bbe
{
	namespace INTERNAL
	{
		namespace vulkan
		{
			class VulkanDevice;
			class VulkanCommandPool;
			class VulkanManager;
			class VulkanDescriptorSet;
		}
	}

	class PrimitiveBrush2D;
	class PrimitiveBrush3D;
	class Terrain;
	class TerrainMesh;
	class TerrainSingle;

	enum class ImageFormat
	{
		R8G8B8A8    = VK_FORMAT_R8G8B8A8_UNORM,
		R8          = VK_FORMAT_R8_UNORM,
		R32FLOAT    = VK_FORMAT_R32_SFLOAT,
		R32FLOATBC6 = VK_FORMAT_BC6H_UFLOAT_BLOCK
	};

	enum class ImageRepeatMode
	{
		REPEAT               = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		MIRRORED_REPEAT      = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
		CLAMP_TO_EDGE        = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER      = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
		MIRROR_CLAMP_TO_EDGE = VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
	};

	enum class ImageFilterMode
	{
		LINEAR  = VK_FILTER_LINEAR,
		NEAREST = VK_FILTER_NEAREST
	};

	class Image
	{
		friend class INTERNAL::vulkan::VulkanManager;
		friend class INTERNAL::vulkan::VulkanDescriptorSet;
		friend class PrimitiveBrush2D;
		friend class PrimitiveBrush3D;
		friend class Terrain;
		friend class TerrainMesh;
		friend class TerrainSingle;
		friend class TerrainTransformed;
	private:
		byte           *m_pdata  = nullptr;
		int             m_width  = 0;
		int             m_height = 0;
		ImageFormat     m_format = ImageFormat::R8G8B8A8;
		ImageRepeatMode m_repeatMode = ImageRepeatMode::REPEAT;
		ImageFilterMode m_filterMode = ImageFilterMode::LINEAR;

		mutable VkImage        m_image       = VK_NULL_HANDLE;
		mutable VkDeviceMemory m_imageMemory = VK_NULL_HANDLE;
		mutable VkImageView    m_imageView   = VK_NULL_HANDLE;
		mutable std::unique_ptr<VkImageLayout[]> m_imageLayout = nullptr;
		mutable VkDevice       m_device      = VK_NULL_HANDLE;
		mutable VkSampler      m_sampler     = VK_NULL_HANDLE;
		mutable INTERNAL::vulkan::VulkanDescriptorSet m_descriptorrSet;
		mutable const Image*   m_parentImage = nullptr;

		mutable bool wasUploadedToVulkan = false;
		void createAndUpload(const INTERNAL::vulkan::VulkanDevice &device, const INTERNAL::vulkan::VulkanCommandPool &commandPool, const INTERNAL::vulkan::VulkanDescriptorPool &descriptorPool, const INTERNAL::vulkan::VulkanDescriptorSetLayout &setLayout, const Image* parentImage = nullptr) const;
		void changeLayout(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkImageLayout layout, uint32_t baseMipLevel = 0, uint32_t levelCount = 1) const;
		void writeBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue queue, VkBuffer buffer) const;

		VkSampler getSampler() const;
		VkImageView getImageView() const;
		VkImageLayout getImageLayout() const;

		INTERNAL::vulkan::VulkanDescriptorSet& getDescriptorSet() const;

	public:
		Image();
		explicit Image(const char* path);
		Image(int width, int height);
		Image(int width, int height, const Color &c);
		Image(int width, int height, const float* data, ImageFormat format);
		
		~Image();

		void load(const char* path);
		void load(int width, int height);
		void load(int width, int height, const Color &c);
		void load(int width, int height, const float* data, ImageFormat format);

		void destroy();

		int getWidth() const;
		int getHeight() const;
		int getSizeInBytes() const;
		int getAmountOfChannels() const;
		int getBytesPerChannel() const;
		Color getPixel(int x, int y) const;

		ImageRepeatMode getRepeatMode() const;
		void setRepeatMode(ImageRepeatMode irm);

		ImageFilterMode getFilterMode() const;
		void setFilterMode(ImageFilterMode ifm);
	};
}