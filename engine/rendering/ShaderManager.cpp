module;

#include <slang/slang.h>
#include <slang/slang-com-helper.h>
#include <slang/slang-com-ptr.h>
export module ShaderManager;
namespace Rendering::Vulkan
{
    /**
     * TODO: Deprecate this class and use a dedicated pipeline manager. for now this will handle File I/O and compiling shaders
     */
    export class ShaderManager
    {
        ShaderManager()
        {

            Slang::ComPtr<slang::IGlobalSession> global_session = nullptr;
            slang::createGlobalSession(&global_session);

            slang::SessionDesc session_desc = {};
            slang::TargetDesc target_desc = {};
            target_desc.format = SLANG_SPIRV;
            target_desc.profile = global_session->findProfile("spirv_1_5");

            session_desc.targets = &target_desc;
            session_desc.targetCount = 1;

            Slang::ComPtr<slang::ISession> session = nullptr;
            global_session->createSession(session_desc, &session);

            slang_session = session;
        }


        ~ShaderManager()
        {
            if (slang_session)
            {
                slang_session->release();
            }
        }
    private:
        slang::ISession* slang_session = nullptr;
    };
    
    
};