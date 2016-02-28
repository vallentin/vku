# vku - Vulkan Utilities

## Introduction

*Vulkan Utilities simplifies a lot of by supplying extension/layer support checking,
easy instance and device creation and much more.*

**Notice:** This library is still very young, so it will be updated frequently/daily,
until everything in Vulkan this library should cover is covered.

Updated for Vulkan 1.0.4.



## Setup

[vku](https://github.com/MrVallentin/vku) is a header only library (this might change in later revisions).
[vku](https://github.com/MrVallentin/vku) doesn't include any Vulkan headers, this means that prior to including
[vku.h](https://github.com/MrVallentin/vku/vlu.h), the appropriate Vulkan related headers needs
to be included. In the following examples [vku](https://github.com/MrVallentin/vku) will be using
[vkel](https://github.com/MrVallentin/vkel), as the resource for dynamically loading Vulkan function pointers.



## Example: Functionality

The following example isn't a full program! It just shows how much everything has been simplified.
Creating a `VkInstance` isn't a hassle anymore, this is taken care of by a simply call to `vkuCreateSimpleInstance()`.


```c
#include <stdio.h> // needed for getchar(), printf() and fprintf()
#include <stdlib> // needed for calloc() and free()

#include "vkel.h"
#include "vku.h"

int main(int argc, char **argv)
{
	if (!vkelInit())
	{
		fprintf(stderr, "Failed to initialize Vulkan\n");
		return -1;
	}
	
	
	VkResult err;
	VkBool32 res;
	
	
	VkBool32 surfaceExtensionSupported =
		vkuIsInstanceExtensionSupported(NULL, VK_KHR_SURFACE_EXTENSION_NAME);
	
	
	VkBool32 platformSurfaceExtensionSupported = VK_FALSE;

#if defined(VK_USE_PLATFORM_WIN32_KHR)
	platformSurfaceExtensionSupported = vkuIsInstanceExtensionSupported(NULL, VK_KHR_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
	platformSurfaceExtensionSupported = vkuIsInstanceExtensionSupported(NULL, VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#else
	// Add more if needed
#endif


	// These extensions are necessary for drawing to a surface
	assert(surfaceExtensionSupported);
	assert(platformSurfaceExtensionSupported);


	const VkBool32 debugReportExtensionSupported =
		vkuIsInstanceExtensionSupported(NULL, VK_EXT_DEBUG_REPORT_EXTENSION_NAME);


	// Set to TRUE/FALSE depending on if validation/error reporting is needed
	VkBool32 enableValidation = VK_FALSE
		&& debugReportExtensionSupported;


	VkInstance instance;

	// err = vkuCreateSimpleInstance(VK_API_VERSION, VK_FALSE, NULL, &instance);
	err = vkuCreateSimpleInstance(VK_MAKE_VERSION(1, 0, 3), VK_FALSE, NULL, &instance);

	if (err)
		printf("vkCreateInstance Error %d: %s\n", err, vkuGetResultString(err));
	assert(!err);

	
	VkPhysicalDevice physicalDevice;
	err = vkuGetPhysicalDevice(instance, &physicalDevice);

	if (err)
		printf("vkCreateInstance Error %d: %s\n", err, vkuGetResultString(err));
	assert(!err);


	VkBool32 swapchainExtensionSupported =
		vkuIsDeviceExtensionSupported(physicalDevice, NULL, VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	// This extension is necessary
	assert(swapchainExtensionSupported);


	uint32_t queueCount;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, NULL);
	assert(queueCount > 0);


	uint32_t queueFamilyIndex;
	res = vkuGetQueueFamilyIndex(physicalDevice, &queueFamilyIndex);
	assert(res);


	VkPhysicalDeviceFeatures physicalDeviceFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &physicalDeviceFeatures);


	VkDevice device;
	err = vkuCreateSimpleDevice(enableValidation, NULL, physicalDevice, queueFamilyIndex, &device);
	
	if (err)
		printf("vkCreateInstance Error %d: %s\n", err, vkuGetResultString(err));
	assert(!err);
	
	
	
	// Vulkan is being simplified, so more is coming soon!
	
	
	
	// Pause, so we get to see something before it exits
	getchar();
	
	
	// Release the Vulkan library again (the OS will also do this automatically of course).
	vkelUninit();
	
	
	return 0;
}
```



## Example: Support Checking


```c
VkInstance instance;
// Do all the stuff to create a VkInstance

VkPhysicalDevice physicalDevice
// Do all the stuff to create a VkPhysicalDevice


if (vkuIsInstanceLayerSupported("VK_LAYER_LUNARG_api_dump"))
	// Instance layer is supported

if (vkuIsInstanceExtensionSupported(NULL, "VK_KHR_win32_surface"))
	// Instance extension is supported

if (vkuIsInstanceExtensionSupported("VK_LAYER_LUNARG_device_limits", "VK_KHR_win32_surface"))
	// Instance extension is supported in layer


if (vkuIsDeviceLayerSupported(physicalDevice, "VK_LAYER_LUNARG_vktrace"))
	// Device layer is supported

if (vkuIsDeviceExtensionSupported(physicalDevice, NULL, "VK_NV_glsl_shader"))
	// Device extension is supported

if (vkuIsInstanceExtensionSupported(physicalDevice, "VK_LAYER_LUNARG_draw_state", "VK_LUNARG_DEBUG_MARKER"))
	// Device extension is supported in layer
```


## Example: List Supported Extensions

[vku](https://github.com/MrVallentin/vku) can also be used to list all
supported extensions and layers using:

- `vkuGetInstanceExtensionNames`
- `vkuGetInstanceLayerNames`
- `vkuGetDeviceExtensionNames`
- `vkuGetDeviceLayerNames`

Individual extensions and layers can be checked using:

- `vkuIsInstanceLayerSupported`
- `vkuIsInstanceExtensionSupported`
- `vkuIsDeviceLayerSupported`
- `vkuIsDeviceExtensionSupported`

```c
#include <stdio.h> // needed for getchar(), printf() and fprintf()
#include <stdlib> // needed for calloc() and free()

#include "vkel.h"
#include "vku.h"

int main(int argc, char **argv)
{
	if (!vkelInit())
	{
		fprintf(stderr, "Failed to initialize Vulkan\n");
		return -1;
	}
	
	
	uint32_t extensionNameCount = 0;
	char **extensionNames = vkuGetInstanceExtensionNames(NULL, &extensionNameCount);
	
	printf("Count: %d\n", extensionNameCount);
	
	for (uint32_t extensionNameIndex = 0; extensionNameIndex < extensionNameCount; extensionNameIndex++)
	{
		printf("Extension %d: %s\n", (extensionNameIndex + 1), extensionNames[extensionNameIndex]);
	}
	
	printf("\n");
	
	vkuDeleteInstanceExtensionNames(extensionNameCount, extensionNames);
	extensionNames = NULL;
	
	
	// Pause, so we get to see something before it exits
	getchar();
	
	
	// Release the Vulkan library again (the OS will also do this automatically of course).
	vkelUninit();
	
	
	return 0;
}
```



## API Reference



### VkInstance Creation

`VkResult vkuCreateSimpleInstance(uint32_t apiVersion, VkBool32 enableValidation, const VkAllocationCallbacks *pAllocator, VkInstance *instance)`

> Creates a `VkInstance` without any hassle. It simply adds all extensions which the system
> supports. Though if `enableValidation` is set to `VK_FALSE`, then `VK_EXT_debug_report` isn't added.
> Further if `enableValidation` is set to `VK_TRUE` then all supported layers are added as well.


```c
VkResult vkuCreateInstance(uint32_t apiVersion,
	uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames,
	uint32_t enabledLayerCount, const char* const* ppEnabledLayerNames,
	const VkAllocationCallbacks *pAllocator,
	VkInstance *instance)
```

> This overall just serves as a shortcut for creating a `VkInstance`.


# VkPhysicalDevice & VkDevice Creation

`VkResult vkuGetPhysicalDevice(VkInstance instance, VkPhysicalDevice *physicalDevice)`
> Get a `VkPhysicalDevice` using an `VkInstance`.

`VkBool32 vkuGetQueueFamilyIndex(VkPhysicalDevice physicalDevice, uint32_t *queueFamilyIndex)`
> Get a QueueFamilyIndex a `VkPhysicalDevice`.


`VkResult vkuCreateSimpleDevice(VkBool32 enableValidation, const VkAllocationCallbacks *pAllocator, VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkDevice *device)`

> Creates a `VkDevice` using a `VkPhysicalDevice` and a QueueFamilyIndex,
> without any hassle. It simply adds all extensions which the system
> supports. Though if `enableValidation` is set to `VK_FALSE`, then `VK_EXT_debug_report` isn't added.
> Further if `enableValidation` is set to `VK_TRUE` then all supported layers are added as well.

```c
VkResult vkuCreateDevice(
	uint32_t enabledExtensionCount, const char* const* ppEnabledExtensionNames,
	uint32_t enabledLayerCount, const char* const* ppEnabledLayerNames,
	const VkAllocationCallbacks *pAllocator,
	VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkDevice *device)
```

> This overall just serves as a shortcut for creating a `VkDevice`.





### Check Supported Extensions/Layers

- `VkBool32 vkuIsInstanceLayerSupported(const char *pLayerName)`
- `VkBool32 vkuIsDeviceLayerSupported(VkPhysicalDevice physicalDevice, const char *pLayerName)`

> Check if instance/device layer is supported.

- `VkBool32 vkuIsInstanceExtensionSupported(const char *pLayerName, const char *pExtensionName)`
- `VkBool32 vkuIsDeviceExtensionSupported(VkPhysicalDevice physicalDevice, const char *pLayerName, const char *pExtensionName)`

> Check if instance/device extension is supported.

*Remember that they can be checking using the extension name itself. With the
minor change of having the prefix `vku_` instead of `VK_`. Example, `VK_KHR_win32_surface` would
be `vku_KHR_win32_surface`.*


### Listing Supported Extensions/Layers

*Check the example above.*

- `char** vkuGetInstanceExtensionNames(const char *pLayerName, uint32_t *extensionNameCount)`
- `char** vkuGetDeviceExtensionNames(VkPhysicalDevice physicalDevice, const char *pLayerName, uint32_t *extensionNameCount)`

> Get an array of all the supported instance/device extension names.

- `char** vkuGetInstanceLayerNames(uint32_t *layerNameCount)`
- `char** vkuGetDeviceLayerNames(VkPhysicalDevice physicalDevice, uint32_t *layerNameCount)`

> Get an array of all the supported instance/device layer names.

- `void vkuDeleteInstanceExtensionNames(uint32_t extensionNameCount, char **extensionNames)`
- `void vkuDeleteInstanceLayerNames(uint32_t layerNameCount, char **layerNames)`
- `void vkuDeleteDeviceExtensionNames(uint32_t extensionNameCount, char **extensionNames)`
- `void vkuDeleteDeviceLayerNames(uint32_t layerNameCount, char **layerNames)`

> The return `char**` can be manually deleted, but the above function exist for simplifying
> the process. The above functions are also just `#define`'s of `vkuDeleteNames()`


### Extra

- `const char* vkuGetResultString(const VkResult err)`
> Converts a `VkResult` to a string (`const char*`). The returned
> string is static, so you do not need to delete it.



## Reporting Bugs & Requests

Feel free to use the [issue tracker](https://github.com/MrVallentin/vku/issues).
Please always include the name and version of the OS where the bug occurs.


## Dependencies

- Vulkan - *If you're using this in the young days of Vulkan, then make sure that you have the Vulkan driver installed, if any problems occur.*
- Windows (header) - needed for library loading on Windows
- Standard C Libraries (stdio, stdlib, string, assert) - needed for NULL, malloc() calloc(), free(), memset(), assert()


### License & Copyright

```
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would
   be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not
   be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
   distribution.
```

#### Additional Copyright

Vulkan™ and the Vulkan logo are trademarks of the Khronos Group Inc.

![Vulkan Logo](http://advvulkan.com/Vulkan_500px_Mar15.png)
