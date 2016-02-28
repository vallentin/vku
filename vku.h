//========================================================================
// Name
//     Vulkan Utilities
//
// Repository
//     https://github.com/VallentinSource/vku
//
// Overview
//     extension loader.
//     Vulkan Utilities simplifies a lot of by supplying
//     extension/layer support checking, easy instance and device
//     creation and much more.
//
// Dependencies
//     Vulkan (library)
//     Windows (header) - needed for library loading on Windows
//     Standard C Libraries (stdio, stdlib, string, assert) - needed for NULL, malloc()
//                                                 calloc(), free(), memset(), assert()
//
// Notice
//     Copyright (c) 2016 Vallentin Source <mail@vallentinsource.com>
//
// Developers & Contributors
//     Christian Vallentin <mail@vallentinsource.com>
//
// Version
//     Last Modified Data: February 27, 2016
//     Revision: 3
//
// Revision History
//     Revision 3, 2016/02/27
//       - Implemented simplified VkDevice creation
//         and getting of QueueFamilyIndex.
//
//     Revision 2, 2016/02/24
//       - Implemented simplified VkInstance creation and
//         getting of VkPhysicalDevice.
//       - Implemented VkResult to string.
//
//     Revision 1, 2016/02/25
//       - Implemented extension and layer support checking.
//
//------------------------------------------------------------------------
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//========================================================================

#pragma once
#ifndef _vulkan_vku_h_
#define _vulkan_vku_h_ 1
#define _VULKAN_VKU_H_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h> /* NULL, printf() */
#include <stdlib.h> /* malloc(), calloc(), free() */
#include <string.h> /* memset() */
#include <assert.h> /* assert() */


// Used for developing, as vkel is
// located next to the vku folder.
// #include "..\vkel\vkel.h"


#if !defined(__vulkan_h_) && !defined(VULKAN_H_)
#	error Vulkan needs to be included prior to including vku.h
#endif


// #define VKUAPI_ATTR
#define VKUAPI_ATTR static



VKUAPI_ATTR const char* vkuGetResultString(const VkResult err)
{
	switch (err)
	{
	case VK_SUCCESS:
		return "Success";
	case VK_NOT_READY:
		return "Not ready";
	case VK_TIMEOUT:
		return "Timeout";
	case VK_EVENT_SET:
		return "Event set";
	case VK_EVENT_RESET:
		return "Event reset";
	case VK_INCOMPLETE:
		return "Incomplete";
	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return "Out of host memory";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return "Out of device memory";
	case VK_ERROR_INITIALIZATION_FAILED:
		return "Initialization failed";
	case VK_ERROR_DEVICE_LOST:
		return "Device lost";
	case VK_ERROR_MEMORY_MAP_FAILED:
		return "Memory map failed";
	case VK_ERROR_LAYER_NOT_PRESENT:
		return "Layer not present";
	case VK_ERROR_EXTENSION_NOT_PRESENT:
		return "Extension not present";
	case VK_ERROR_FEATURE_NOT_PRESENT:
		return "Feature not present";
	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return "Incompatible driver";
	case VK_ERROR_TOO_MANY_OBJECTS:
		return "Too many objects";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return "Format not supported";
	case VK_ERROR_SURFACE_LOST_KHR:
		return "Surface lost";
	case VK_SUBOPTIMAL_KHR:
		return "Suboptimal";
	case VK_ERROR_OUT_OF_DATE_KHR:
		return "Out of date";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return "Incompatible display";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return "Native window in use";
	case VK_ERROR_VALIDATION_FAILED_EXT:
		return "Validation failed";
	default:
		return "Unknown error";
	}
}



static int vku_strcmp(const char *str1, const char *str2)
{
	while (*str1 && (*str1 == *str2))
	{
		str1++, str2++;
	}

	return *(const unsigned char*) str1 - *(const unsigned char*) str2;
}

static void vku_strpy(char *dest, char *src)
{
	while (*dest++ = *src++);
}



VKUAPI_ATTR void vkuDeleteNames(uint32_t nameCount, char **names)
{
	// assert(names);
	if (!names)
		return;

	for (uint32_t nameIndex = 0; nameIndex < nameCount; nameIndex++)
	{
		// assert(names[nameIndex]);
		if (!names[nameIndex])
			continue;

		free(names[nameIndex]);
	}

	free(names);
}


#define vkuDeleteInstanceExtensionNames vkuDeleteNames
#define vkuDeleteInstanceLayerNames vkuDeleteNames
#define vkuDeleteDeviceExtensionNames vkuDeleteNames
#define vkuDeleteDeviceLayerNames vkuDeleteNames


VKUAPI_ATTR char** vkuGetInstanceExtensionNames(const char *pLayerName, uint32_t *extensionNameCount)
{
	assert(extensionNameCount);


	VkResult err;


	uint32_t extPropertyCount;
	err = vkEnumerateInstanceExtensionProperties(pLayerName, &extPropertyCount, NULL);
	assert(!err);

	(*extensionNameCount) = extPropertyCount;

	if (extPropertyCount < 1)
		return NULL;


	char **extensionNames = (char**) calloc(extPropertyCount, sizeof(char*));

	for (uint32_t extensionNameIndex = 0; extensionNameIndex < extPropertyCount; extensionNameIndex++)
		extensionNames[extensionNameIndex] = (char*) calloc(VK_MAX_EXTENSION_NAME_SIZE, sizeof(char));


	VkExtensionProperties *extProperties = (VkExtensionProperties*) calloc(extPropertyCount, sizeof(VkExtensionProperties));
	assert(extProperties);


	err = vkEnumerateInstanceExtensionProperties(pLayerName, &extPropertyCount, extProperties);
	assert(!err);

	for (uint32_t extPropertyIndex = 0; extPropertyIndex < extPropertyCount; extPropertyIndex++)
		vku_strpy(extensionNames[extPropertyIndex], extProperties[extPropertyIndex].extensionName);


	free(extProperties);


	return extensionNames;
}

VKUAPI_ATTR char** vkuGetDeviceExtensionNames(VkPhysicalDevice physicalDevice, const char *pLayerName, uint32_t *extensionNameCount)
{
	assert(extensionNameCount);


	VkResult err;


	uint32_t extPropertyCount;
	err = vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, &extPropertyCount, NULL);
	assert(!err);

	(*extensionNameCount) = extPropertyCount;

	if (extPropertyCount < 1)
		return NULL;


	char **extensionNames = (char**) calloc(extPropertyCount, sizeof(char*));

	for (uint32_t extensionNameIndex = 0; extensionNameIndex < extPropertyCount; extensionNameIndex++)
		extensionNames[extensionNameIndex] = (char*) calloc(VK_MAX_EXTENSION_NAME_SIZE, sizeof(char));


	VkExtensionProperties *extProperties = (VkExtensionProperties*) calloc(extPropertyCount, sizeof(VkExtensionProperties));
	assert(extProperties);


	err = vkEnumerateDeviceExtensionProperties(physicalDevice, pLayerName, &extPropertyCount, extProperties);
	assert(!err);

	for (uint32_t extPropertyIndex = 0; extPropertyIndex < extPropertyCount; extPropertyIndex++)
		vku_strpy(extensionNames[extPropertyIndex], extProperties[extPropertyIndex].extensionName);


	free(extProperties);


	return extensionNames;
}


VKUAPI_ATTR char** vkuGetInstanceLayerNames(uint32_t *layerNameCount)
{
	assert(layerNameCount);


	VkResult err;


	uint32_t layerPropertyCount;
	err = vkEnumerateInstanceLayerProperties(&layerPropertyCount, NULL);
	assert(!err);

	(*layerNameCount) = layerPropertyCount;

	if (layerPropertyCount < 1)
		return NULL;


	char **layerNames = (char**) calloc(layerPropertyCount, sizeof(char*));

	for (uint32_t layerNameIndex = 0; layerNameIndex < layerPropertyCount; layerNameIndex++)
		layerNames[layerNameIndex] = (char*) calloc(VK_MAX_EXTENSION_NAME_SIZE, sizeof(char));


	VkLayerProperties *layerProperties = (VkLayerProperties*) calloc(layerPropertyCount, sizeof(VkLayerProperties));
	assert(layerProperties);

	err = vkEnumerateInstanceLayerProperties(&layerPropertyCount, layerProperties);
	assert(!err);

	for (uint32_t layerPropertyIndex = 0; layerPropertyIndex < layerPropertyCount; layerPropertyIndex++)
		vku_strpy(layerNames[layerPropertyIndex], layerProperties[layerPropertyIndex].layerName);


	free(layerProperties);


	return layerNames;
}

VKUAPI_ATTR char** vkuGetDeviceLayerNames(VkPhysicalDevice physicalDevice, uint32_t *layerNameCount)
{
	assert(layerNameCount);


	VkResult err;


	uint32_t layerPropertyCount;
	err = vkEnumerateDeviceLayerProperties(physicalDevice, &layerPropertyCount, NULL);
	assert(!err);

	(*layerNameCount) = layerPropertyCount;

	if (layerPropertyCount < 1)
		return NULL;


	char **layerNames = (char**) calloc(layerPropertyCount, sizeof(char*));

	for (uint32_t layerNameIndex = 0; layerNameIndex < layerPropertyCount; layerNameIndex++)
		layerNames[layerNameIndex] = (char*) calloc(VK_MAX_EXTENSION_NAME_SIZE, sizeof(char));


	VkLayerProperties *layerProperties = (VkLayerProperties*) calloc(layerPropertyCount, sizeof(VkLayerProperties));
	assert(layerProperties);

	err = vkEnumerateDeviceLayerProperties(physicalDevice, &layerPropertyCount, layerProperties);
	assert(!err);

	for (uint32_t layerPropertyIndex = 0; layerPropertyIndex < layerPropertyCount; layerPropertyIndex++)
		vku_strpy(layerNames[layerPropertyIndex], layerProperties[layerPropertyIndex].layerName);


	free(layerProperties);


	return layerNames;
}



VKUAPI_ATTR VkBool32 vkuIsInstanceExtensionSupported(const char *pLayerName, const char *pExtensionName)
{
	uint32_t extensionNameCount = 0;
	char **extensionNames = vkuGetInstanceExtensionNames(pLayerName, &extensionNameCount);

	for (uint32_t extensionNameIndex = 0; extensionNameIndex < extensionNameCount; extensionNameIndex++)
	{
		if (vku_strcmp(extensionNames[extensionNameIndex], pExtensionName))
		{
			vkuDeleteInstanceExtensionNames(extensionNameCount, extensionNames);

			return VK_TRUE;
		}
	}

	vkuDeleteInstanceExtensionNames(extensionNameCount, extensionNames);

	return VK_FALSE;
}

VKUAPI_ATTR VkBool32 vkuIsDeviceExtensionSupported(VkPhysicalDevice physicalDevice, const char *pLayerName, const char *pExtensionName)
{
	uint32_t extensionNameCount = 0;
	char **extensionNames = vkuGetDeviceExtensionNames(physicalDevice, pLayerName, &extensionNameCount);

	for (uint32_t extensionNameIndex = 0; extensionNameIndex < extensionNameCount; extensionNameIndex++)
	{
		if (vku_strcmp(extensionNames[extensionNameIndex], pExtensionName))
		{
			vkuDeleteDeviceExtensionNames(extensionNameCount, extensionNames);

			return VK_TRUE;
		}
	}

	vkuDeleteDeviceExtensionNames(extensionNameCount, extensionNames);

	return VK_FALSE;
}


VKUAPI_ATTR VkBool32 vkuIsInstanceLayerSupported(const char *pLayerName)
{
	uint32_t layerNameCount = 0;
	char **layerNames = vkuGetInstanceLayerNames(&layerNameCount);

	for (uint32_t layerNameIndex = 0; layerNameIndex < layerNameCount; layerNameIndex++)
	{
		if (vku_strcmp(layerNames[layerNameIndex], pLayerName))
		{
			vkuDeleteInstanceLayerNames(layerNameCount, layerNames);

			return VK_TRUE;
		}
	}

	vkuDeleteInstanceLayerNames(layerNameCount, layerNames);

	return VK_FALSE;
}

VKUAPI_ATTR VkBool32 vkuIsDeviceLayerSupported(VkPhysicalDevice physicalDevice, const char *pLayerName)
{
	uint32_t layerNameCount = 0;
	char **layerNames = vkuGetDeviceLayerNames(physicalDevice, &layerNameCount);

	for (uint32_t layerNameIndex = 0; layerNameIndex < layerNameCount; layerNameIndex++)
	{
		if (vku_strcmp(layerNames[layerNameIndex], pLayerName))
		{
			vkuDeleteDeviceLayerNames(layerNameCount, layerNames);

			return VK_TRUE;
		}
	}

	vkuDeleteDeviceLayerNames(layerNameCount, layerNames);

	return VK_FALSE;
}



VKUAPI_ATTR VkResult vkuCreateInstance(uint32_t apiVersion,
	uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames,
	uint32_t enabledLayerCount, const char* const* ppEnabledLayerNames,
	const VkAllocationCallbacks *pAllocator,
	VkInstance *instance)
{
	VkApplicationInfo appInfo;
	memset(&appInfo, 0, sizeof(appInfo));

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "VulkanApp";
	appInfo.pEngineName = "VulkanEngine";
	appInfo.apiVersion = apiVersion;


	VkInstanceCreateInfo instCreateInfo;
	memset(&instCreateInfo, 0, sizeof(instCreateInfo));

	instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instCreateInfo.pNext = NULL;
	instCreateInfo.pApplicationInfo = &appInfo;


	instCreateInfo.enabledExtensionCount = enabledExtensionCount;
	instCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;

	instCreateInfo.enabledLayerCount = enabledLayerCount;
	instCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames;


	return vkCreateInstance(&instCreateInfo, pAllocator, instance);
}

VKUAPI_ATTR VkResult vkuCreateSimpleInstance(uint32_t apiVersion, VkBool32 enableValidation, const VkAllocationCallbacks *pAllocator, VkInstance *instance)
{
	uint32_t enabledExtensionCount = 0;
	char **enabledExtensionNames = vkuGetInstanceExtensionNames(NULL, &enabledExtensionCount);

	uint32_t actualExtensionCount = enabledExtensionCount;


	uint32_t enabledLayerCount = 0;
	char **enabledLayerNames = NULL;

	uint32_t actualLayerCount = enabledLayerCount;


	if (enableValidation)
	{
		// Validation is enabled, so just add all the layers

		enabledLayerNames = vkuGetInstanceLayerNames(&enabledLayerCount);
		actualLayerCount = enabledLayerCount;
	}
	else
	{
		// Validation is not enabled, so remove the VK_EXT_debug_report extension

		for (uint32_t enabledExtensionIndex = 0; enabledExtensionIndex < actualExtensionCount; enabledExtensionIndex++)
		{
			char *enabledExtensionName = enabledExtensionNames[enabledExtensionIndex];

			if (!vku_strcmp(enabledExtensionName, VK_EXT_DEBUG_REPORT_EXTENSION_NAME))
			{
				// If it's the last extension name, just decrement enabledExtensionCount
				if (enabledExtensionIndex == (actualExtensionCount - 1))
				{
					enabledExtensionCount--;
				}
				else // If not, swap with the last extension name
				{
					enabledExtensionNames[enabledExtensionIndex] = enabledExtensionNames[actualExtensionCount - 1];
					enabledExtensionNames[actualExtensionCount - 1] = enabledExtensionName;

					// The first if will be called in the end, decrementing the enabledExtensionCount
				}
			}
		}
	}


	VkResult err = vkuCreateInstance(apiVersion, enabledExtensionCount, enabledExtensionNames, enabledLayerCount, enabledLayerNames, pAllocator, instance);


	// Remember to delete it by the actualExtensionCount and not the enabledExtensionCount
	vkuDeleteInstanceExtensionNames(actualExtensionCount, enabledExtensionNames);
	// enabledExtensionNames = NULL;

	// Remember to delete it by the actualLayerCount and not the enabledLayerCount
	vkuDeleteInstanceLayerNames(actualLayerCount, enabledLayerNames);
	// enabledLayerNames = NULL;


	return err;
}



VKUAPI_ATTR VkResult vkuGetPhysicalDevice(VkInstance instance, VkPhysicalDevice *physicalDevice)
{
	assert(physicalDevice);


	uint32_t gpuCount = 0;
	VkResult err = vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);
	assert(!err);
	assert(gpuCount > 0);

	// printf("GPUs: %d\n", gpuCount);

	VkPhysicalDevice *physicalDevices = (VkPhysicalDevice*) calloc(gpuCount, sizeof(VkPhysicalDevice));
	assert(physicalDevices);

	err = vkEnumeratePhysicalDevices(instance, &gpuCount, physicalDevices);
	assert(!err);

	// Right now, just use the first encountered physical device
	(*physicalDevice) = physicalDevices[0];

	free(physicalDevices);

	return err;
}



VKUAPI_ATTR VkBool32 vkuGetQueueFamilyIndex(VkPhysicalDevice physicalDevice, uint32_t *queueFamilyIndex)
{
	assert(queueFamilyIndex);


	uint32_t queueCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, NULL);
	assert(queueCount > 0);

	VkQueueFamilyProperties *queueFamilyProperties = (VkQueueFamilyProperties*) calloc(queueCount, sizeof(VkQueueFamilyProperties));
	assert(queueFamilyProperties);

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueFamilyProperties);

	for ((*queueFamilyIndex) = 0; (*queueFamilyIndex) < queueCount; (*queueFamilyIndex)++)
		if (queueFamilyProperties[(*queueFamilyIndex)].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			break;

	// assert((*queueFamilyIndex) < queueCount);


	free(queueFamilyProperties);


	if ((*queueFamilyIndex) < queueCount)
		return VK_TRUE;

	return VK_FALSE;
}



VKUAPI_ATTR VkResult vkuCreateDevice(
	uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames,
	uint32_t enabledLayerCount, const char* const* ppEnabledLayerNames,
	const VkAllocationCallbacks *pAllocator,
	VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkDevice *device)
{
	assert(device);


	VkDeviceQueueCreateInfo queueCreateInfo;
	memset(&queueCreateInfo, 0, sizeof(queueCreateInfo));

	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndex;

	const uint32_t queuePrioritiesCount = 1;
	const float queuePriorities[1] = { 0.0f };

	queueCreateInfo.queueCount = queuePrioritiesCount;
	queueCreateInfo.pQueuePriorities = queuePriorities;


	VkDeviceCreateInfo deviceCreateInfo;
	memset(&deviceCreateInfo, 0, sizeof(deviceCreateInfo));

	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = NULL;

	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;

	deviceCreateInfo.pEnabledFeatures = NULL;

	deviceCreateInfo.enabledExtensionCount = enabledExtensionCount;
	deviceCreateInfo.ppEnabledExtensionNames = ppEnabledExtensionNames;

	deviceCreateInfo.enabledLayerCount = enabledLayerCount;
	deviceCreateInfo.ppEnabledLayerNames = ppEnabledLayerNames;


	return vkCreateDevice(physicalDevice, &deviceCreateInfo, pAllocator, device);
}

VKUAPI_ATTR VkResult vkuCreateSimpleDevice(VkBool32 enableValidation, const VkAllocationCallbacks *pAllocator, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkDevice *device)
{
	assert(device);


	uint32_t enabledExtensionCount = 0;
	char **enabledExtensionNames = vkuGetDeviceExtensionNames(physicalDevice, NULL, &enabledExtensionCount);

	uint32_t actualExtensionCount = enabledExtensionCount;


	uint32_t enabledLayerCount = 0;
	char **enabledLayerNames = NULL;

	uint32_t actualLayerCount = enabledLayerCount;


	if (enableValidation)
	{
		// Validation is enabled, so just add all the layers

		enabledLayerNames = vkuGetDeviceLayerNames(physicalDevice, &enabledLayerCount);
		actualLayerCount = enabledLayerCount;
	}
	else
	{
		// Validation is not enabled, so remove the VK_EXT_debug_report extension

		for (uint32_t enabledExtensionIndex = 0; enabledExtensionIndex < actualExtensionCount; enabledExtensionIndex++)
		{
			char *enabledExtensionName = enabledExtensionNames[enabledExtensionIndex];

			if (!vku_strcmp(enabledExtensionName, VK_EXT_DEBUG_REPORT_EXTENSION_NAME))
			{
				// If it's the last extension name, just decrement enabledExtensionCount
				if (enabledExtensionIndex == (actualExtensionCount - 1))
				{
					enabledExtensionCount--;
				}
				else // If not, swap with the last extension name
				{
					enabledExtensionNames[enabledExtensionIndex] = enabledExtensionNames[actualExtensionCount - 1];
					enabledExtensionNames[actualExtensionCount - 1] = enabledExtensionName;

					// The first if will be called in the end, decrementing the enabledExtensionCount
				}
			}
		}
	}


	VkResult err = vkuCreateDevice(enabledExtensionCount, enabledExtensionNames, enabledLayerCount, enabledLayerNames, pAllocator, physicalDevice, queueFamilyIndex, device);


	// Remember to delete it by the actualExtensionCount and not the enabledExtensionCount
	vkuDeleteInstanceExtensionNames(actualExtensionCount, enabledExtensionNames);
	// enabledExtensionNames = NULL;

	// Remember to delete it by the actualLayerCount and not the enabledLayerCount
	vkuDeleteInstanceLayerNames(actualLayerCount, enabledLayerNames);
	// enabledLayerNames = NULL;


	return err;
}



// #if defined(VK_USE_PLATFORM_WIN32_KHR)
// #elif defined(VK_USE_PLATFORM_XCB_KHR)
// #else
// Add more if needed
// #endif



#ifdef __cplusplus
}
#endif

#endif