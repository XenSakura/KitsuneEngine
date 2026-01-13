module;

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>


export module WindowManager;
import std;
import ServiceLocator;
namespace Rendering
{
    /**
     * Window Manager class for handling multiple GLFW windows
     */
    export class WindowManager : public ISystem
    {
    public:
        WindowManager() :window(nullptr)
        {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

            window = glfwCreateWindow(1, 1, "AngelBase", nullptr, nullptr);
        }

        ~WindowManager()
        {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
        
        void ResizeWindow(uint32_t width, uint32_t height)
        {
            glfwSetWindowSize(window, width, height);
        }

        GLFWwindow* GetWindow() const
        {
            return window;
        }
        
        vk::SurfaceKHR VulkanCreateWindowSurface(const vk::Instance& instance) const
        {
            VkSurfaceKHR surface = VK_NULL_HANDLE;
            if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
                throw std::runtime_error("Failed to create window surface");

            return vk::SurfaceKHR(surface);
        }

        VkExtent3D VulkanGetWindowDimensions() const
        {
            int width = 0, height = 0;
            glfwGetWindowSize(window,&width, &height);
            return VkExtent3D(width, height, 1);
        }
    private:
        GLFWwindow* window;
        
    };
}
