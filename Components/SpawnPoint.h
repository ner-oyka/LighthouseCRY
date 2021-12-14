//Deer Solar Games | 2021 | Lighthouse project

#pragma once

#include <CryEntitySystem/IEntitySystem.h>

////////////////////////////////////////////////////////
// Spawn point
////////////////////////////////////////////////////////
class CSpawnPointComponent final : public IEntityComponent
{
public:
	CSpawnPointComponent() = default;
	virtual ~CSpawnPointComponent() = default;

	virtual void Initialize() override;
	virtual ComponentEventPriority GetEventPriority() const override final;
	virtual Cry::Entity::EventFlags GetEventMask() const override;
	virtual void ProcessEvent(const SEntityEvent& event) override;

	// Reflect type to set a unique identifier for this component
	// and provide additional information to expose it in the sandbox
	static void ReflectType(Schematyc::CTypeDesc<CSpawnPointComponent>& desc)
	{
		desc.SetGUID("{41316132-8A1E-4073-B0CD-A242FD3D2E90}"_cry_guid);
		desc.SetEditorCategory("Game");
		desc.SetLabel("SpawnPoint");
		desc.SetDescription("This spawn point can be used to spawn entities");
		desc.SetComponentFlags({ IEntityComponent::EFlags::Transform, IEntityComponent::EFlags::Socket, IEntityComponent::EFlags::Attach });

		desc.AddMember(&CSpawnPointComponent::m_firstEntry, 'fste', "FirstEntry", "First Entry", "First entry.", false);
	}

	static Matrix34 GetFirstSpawnPointTransform()
	{
		// Spawn at first default spawner
		IEntityItPtr pEntityIterator = gEnv->pEntitySystem->GetEntityIterator();
		pEntityIterator->MoveFirst();

		while (!pEntityIterator->IsEnd())
		{
			IEntity* pEntity = pEntityIterator->Next();

			if (CSpawnPointComponent* pSpawner = pEntity->GetComponent<CSpawnPointComponent>())
			{
				if (pSpawner->GetFirstEntry())
				{
					return pSpawner->GetWorldTransformMatrix();
				}
			}
		}

		return IDENTITY;
	}

	static void RemoveSpawnPoint()
	{
		IEntityItPtr pEntityIterator = gEnv->pEntitySystem->GetEntityIterator();
		pEntityIterator->MoveFirst();

		while (!pEntityIterator->IsEnd())
		{
			IEntity* pEntity = pEntityIterator->Next();

			if (CSpawnPointComponent* pSpawner = pEntity->GetComponent<CSpawnPointComponent>())
			{
				gEnv->pEntitySystem->RemoveEntity(pSpawner->GetEntityId());
			}
		}
	}

	bool GetFirstEntry()
	{
		return m_firstEntry;
	}

	void SetFirstEntry(bool val)
	{
		m_firstEntry = val;
	}

private:
	bool m_firstEntry = false;
};
