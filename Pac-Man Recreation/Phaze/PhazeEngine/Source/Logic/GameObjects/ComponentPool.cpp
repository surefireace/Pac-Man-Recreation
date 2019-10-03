#include "ComponentPool.h"
#include "../../Utils/PhazeLogger.h"
#include "MovementComponent.h"
#include "TextureComponent.h"
#include "PhysicsComponent.h"

// TODO: make configurable
// statics
static const std::string s_kLogOutputDir = "Logs/ComponentPoolLog.txt";
const int ComponentPool::s_kComponentLimit = 2000;

void ComponentPool::DeleteComponent(unsigned int componentID)
{
    // find the component
    auto compIter = m_components.find(componentID);

    // sanity check -> component exists
    if (compIter != m_components.end())
    {
        // destroy the component
        compIter->second.~shared_ptr();
    }
}

bool ComponentPool::Init()
{
    // allocate the pool
    m_components.reserve(s_kComponentLimit);
    
    return true;
}

IComponent* ComponentPool::CreateComponent(const Type& type)
{
    // sanity check -> component limit
    if (m_components.size() == s_kComponentLimit)
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, s_kLogOutputDir, "Component limit of: %d reached!", s_kComponentLimit);

    // find the component type then create it
    switch (type)
    {
        case Type::kMove:
        {
            // create the shared pointer
            ComponentPtr sharedptrNewComponent(new MovementComponent);

            // setup raw pointer
            IComponent* pComponent = sharedptrNewComponent.get();

            // TODO: generate id somehow
            // set the component id
            pComponent->SetID(static_cast<unsigned int>(Type::kMove));

            // add to pool
            m_components.insert({ pComponent->GetID(), sharedptrNewComponent });

            // return raw pointer
            return pComponent;

            break;
        }

        case Type::kTexture:
        {
            // create the shared pointer
            ComponentPtr sharedptrNewComponent(new TextureComponent);

            // setup raw pointer
            IComponent* pComponent = sharedptrNewComponent.get();

            // TODO: generate id somehow
            // set the component id
            pComponent->SetID(static_cast<unsigned int>(Type::kTexture));

            // add to pool
            m_components.insert({ pComponent->GetID(), sharedptrNewComponent });

            // return raw pointer
            return pComponent;

            break;
        }

        case Type::kPhysics:
        {
            // create the shared pointer
            ComponentPtr sharedptrNewComponent(new PhysicsComponent);

            // setup raw pointer
            IComponent* pComponent = sharedptrNewComponent.get();

            // TODO: generate id somehow
            // set the component id
            pComponent->SetID(static_cast<unsigned int>(Type::kPhysics));

            // add to pool
            m_components.insert({ pComponent->GetID(), sharedptrNewComponent });

            // return raw pointer
            return pComponent;

            break;
        }

        default:
        {
            break;
        }
    }
    return nullptr;
}
