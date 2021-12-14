#include "StdAfx.h"
#include "BehaviorTreeNodes_Assistant.h"

#include "CryAISystem/BehaviorTree/BehaviorTreeDefines.h"
#include <CryAISystem/BehaviorTree/IBehaviorTree.h>
#include <CryAISystem/BehaviorTree/Action.h>
#include <CryCore/Containers/VariableCollection.h>

#include <CrySerialization/SerializationUtils.h>
#include <CryString/CryName.h>
#include <CryGame/IGameFramework.h>

#include "Framework/PlayerController.h"
#include "Components/Assistant.h"
#include "Pawns/Alex.h"

#include <CryMath/Random.h>

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
#include <CrySerialization/Enum.h>
#include <CrySerialization/ClassFactory.h>
#endif


namespace BehaviorTree
{
	class MoveToPlayer : public Action
	{
		typedef Action BaseClass;

	public:

		struct RuntimeData
		{
		};

		MoveToPlayer()
			: m_pAgentEntity(nullptr)
			, m_pAssistant(nullptr)
		{
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const struct LoadContext& context, const bool isLoadingFromEditor)
		{
			IF_UNLIKELY(BaseClass::LoadFromXml(xml, context, isLoadingFromEditor) == LoadFailure)
			{
				return LoadFailure;
			}
			return LoadSuccess;
		}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
		virtual XmlNodeRef CreateXmlDescription() override
		{
			XmlNodeRef xml = BaseClass::CreateXmlDescription();
			xml->setTag("MoveToPlayer");
			return xml;
		}
#endif

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
		virtual void Serialize(Serialization::IArchive& archive) override
		{
			BaseClass::Serialize(archive);
		}
#endif

		virtual void OnInitialize(const UpdateContext& context)
		{
			//RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			m_pAgentEntity = gEnv->pEntitySystem->GetEntity(context.entityId);
			m_pAssistant = m_pAgentEntity->GetComponent<CAssistantComponent>();
		}

		virtual Status Update(const UpdateContext& context)
		{
			//RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			if (m_pAgentEntity)
			{
				bool completed = m_pAssistant->MoveToPlayer();
				if (completed)
				{
					return Success;
				}
			}
			return Running;
		}

#ifdef DEBUG_MODULAR_BEHAVIOR_TREE
		virtual void GetCustomDebugText(const UpdateContext& updateContext, stack_string& debugText) const
		{
			//const RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(updateContext);
		}
#endif // DEBUG_MODULAR_BEHAVIOR_TREE

	private:
		CAssistantComponent* m_pAssistant;
		IEntity* m_pAgentEntity;
	};


	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//SEARCHING
	class Searching : public Action
	{
		typedef Action BaseClass;

	public:

		struct RuntimeData
		{
		};

		Searching()
			: m_pAgentEntity(nullptr)
			, m_pAssistant(nullptr)
			, m_searchPos(ZERO)
		{
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const struct LoadContext& context, const bool isLoadingFromEditor)
		{
			IF_UNLIKELY(BaseClass::LoadFromXml(xml, context, isLoadingFromEditor) == LoadFailure)
			{
				return LoadFailure;
			}
			return LoadSuccess;
		}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
		virtual XmlNodeRef CreateXmlDescription() override
		{
			XmlNodeRef xml = BaseClass::CreateXmlDescription();
			xml->setTag("Searching");
			return xml;
		}
#endif

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
		virtual void Serialize(Serialization::IArchive& archive) override
		{
			BaseClass::Serialize(archive);
		}
#endif

		virtual void OnInitialize(const UpdateContext& context)
		{
			//RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			m_pAgentEntity = gEnv->pEntitySystem->GetEntity(context.entityId);
			m_pAssistant = m_pAgentEntity->GetComponent<CAssistantComponent>();

			m_searchPos = m_pAgentEntity->GetWorldPos() + Vec3(cry_random(-10.0f, 10.0f), cry_random(-10.0f, 10.0f), 0);
		}

		virtual Status Update(const UpdateContext& context)
		{
			//RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			if (m_pAgentEntity)
			{
				bool completed = m_pAssistant->Searching(m_searchPos);
				if (completed)
				{
					return Success;
				}
			}
			return Running;
		}

#ifdef DEBUG_MODULAR_BEHAVIOR_TREE
		virtual void GetCustomDebugText(const UpdateContext& updateContext, stack_string& debugText) const
		{
			//const RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(updateContext);
		}
#endif // DEBUG_MODULAR_BEHAVIOR_TREE

	private:
		CAssistantComponent* m_pAssistant;
		IEntity* m_pAgentEntity;
		Vec3 m_searchPos;
	};

	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//MOVE TO SEARCH LOCATION
	class MoveToLocation : public Action
	{
		typedef Action BaseClass;

	public:

		struct RuntimeData
		{
		};

		MoveToLocation()
			: m_pAgentEntity(nullptr)
			, m_pAssistant(nullptr)
			, m_searchPos(ZERO)
		{
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const struct LoadContext& context, const bool isLoadingFromEditor)
		{
			IF_UNLIKELY(BaseClass::LoadFromXml(xml, context, isLoadingFromEditor) == LoadFailure)
			{
				return LoadFailure;
			}
			return LoadSuccess;
		}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
		virtual XmlNodeRef CreateXmlDescription() override
		{
			XmlNodeRef xml = BaseClass::CreateXmlDescription();
			xml->setTag("MoveToLocation");
			return xml;
		}
#endif

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
		virtual void Serialize(Serialization::IArchive& archive) override
		{
			BaseClass::Serialize(archive);
		}
#endif

		virtual void OnInitialize(const UpdateContext& context)
		{
			//RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			m_pAgentEntity = gEnv->pEntitySystem->GetEntity(context.entityId);
			m_pAssistant = m_pAgentEntity->GetComponent<CAssistantComponent>();
			m_searchPos = CPlayerController::Get()->GetControlledPawn()->GetEntity()->GetComponent<CAlexPlayer>()->GetCursorWorldPos();

			m_searchPos = m_pAssistant->FindReachablePosition(m_searchPos);

			CPlayerController::Get()->GetControlledPawn()->GetEntity()->GetComponent<CAlexPlayer>()->RemoveTargetMeshToAssistant();
		}

		virtual Status Update(const UpdateContext& context)
		{
			//RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			if (m_pAgentEntity)
			{
				bool completed = m_pAssistant->MoveToLocation(m_searchPos);
				if (completed)
				{
					return Success;
				}
			}
			return Running;
		}

#ifdef DEBUG_MODULAR_BEHAVIOR_TREE
		virtual void GetCustomDebugText(const UpdateContext& updateContext, stack_string& debugText) const
		{
			//const RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(updateContext);
		}
#endif // DEBUG_MODULAR_BEHAVIOR_TREE

	private:
		CAssistantComponent* m_pAssistant;
		IEntity* m_pAgentEntity;
		Vec3 m_searchPos;
	};


	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	//Send DRS Signals
	class SendSignalDRS : public Action
	{
		typedef Action BaseClass;

	public:

		struct RuntimeData
		{
		};

		SendSignalDRS()
			: m_pAgentEntity(nullptr)
			, m_pAssistant(nullptr)
		{
		}

		virtual LoadResult LoadFromXml(const XmlNodeRef& xml, const struct LoadContext& context, const bool isLoadingFromEditor)
		{
			IF_UNLIKELY(BaseClass::LoadFromXml(xml, context, isLoadingFromEditor) == LoadFailure)
			{
				return LoadFailure;
			}

			const stack_string eventName = xml->getAttr("name");
			m_eventToDRS = eventName.c_str();

			return LoadSuccess;
		}

#ifdef USING_BEHAVIOR_TREE_XML_DESCRIPTION_CREATION
		virtual XmlNodeRef CreateXmlDescription() override
		{
			XmlNodeRef xml = BaseClass::CreateXmlDescription();
			xml->setTag("SendSignalDRS");
			xml->setAttr("name", m_eventToDRS);
			return xml;
		}
#endif

#ifdef USING_BEHAVIOR_TREE_SERIALIZATION
		virtual void Serialize(Serialization::IArchive& archive) override
		{
			archive(m_eventToDRS, "eventToDRS", "^EventToDRS");
			archive.doc("Send event to DRS");

			BaseClass::Serialize(archive);
		}
#endif

		virtual void OnInitialize(const UpdateContext& context)
		{
			//RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			m_pAgentEntity = gEnv->pEntitySystem->GetEntity(context.entityId);
			m_pAssistant = m_pAgentEntity->GetComponent<CAssistantComponent>();

			m_pAssistant->SendDRSSignal(m_eventToDRS);
		}

		virtual Status Update(const UpdateContext& context)
		{
			//RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(context);
			return Success;
		}

#ifdef DEBUG_MODULAR_BEHAVIOR_TREE
		virtual void GetCustomDebugText(const UpdateContext& updateContext, stack_string& debugText) const
		{
			//const RuntimeData& runtimeData = GetRuntimeData<RuntimeData>(updateContext);
		}
#endif // DEBUG_MODULAR_BEHAVIOR_TREE

	private:
		CAssistantComponent* m_pAssistant;
		IEntity* m_pAgentEntity;
		string m_eventToDRS;
	};


	void RegisterBehaviorTreeNodes_Assistant()
	{
		IBehaviorTreeManager& manager = *gEnv->pAISystem->GetIBehaviorTreeManager();

		CRY_ASSERT(&manager);

		CRY_DISABLE_WARN_UNUSED_VARIABLES();
		const char* COLOR_GAME = "ff00ff";
		CRY_RESTORE_WARN_UNUSED_VARIABLES();

		REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(manager, MoveToPlayer, "Assistant\\MoveToPlayer", COLOR_GAME);
		REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(manager, Searching, "Assistant\\Searching", COLOR_GAME);
		REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(manager, SendSignalDRS, "Assistant\\SendSignalDRS", COLOR_GAME);
		REGISTER_BEHAVIOR_TREE_NODE_WITH_SERIALIZATION(manager, MoveToLocation, "Assistant\\MoveToLocation", COLOR_GAME);
	}
}