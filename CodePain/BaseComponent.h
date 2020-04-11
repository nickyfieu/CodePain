#pragma once

namespace cp
{
	// Component type is used for safe runetime casting
	// When adding a new component make sure to add a new enum to this component type enum
	// Then in the constructor of the new component make sure to initialize m_ComponentType with the CORRECT type!
	// After that add the new enum to the switch case in the Entity.cpp
	// Cast it to the correct type in Entity.cpp
	// Call the update on the casted component in Entity.cpp
	// In Components.h add an include

	enum class ComponentType
	{
		_Texture2D,
		_Transform,
		_Text,
		_FrameRate,
		_CollisionBox,
		_ColRect2D,
	};

	// BaseComponent is a pure virtual class
	// It only holds a component type object wich is used for safe casting

	class BaseComponent
	{
	public:
		explicit BaseComponent(ComponentType type)
			: m_ComponentType{ type }
		{

		}

		virtual ~BaseComponent() = default;

		virtual void Update(float elapsedSec) = 0;

		BaseComponent(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) = delete;
		BaseComponent& operator= (const BaseComponent&) = delete;
		BaseComponent& operator= (const BaseComponent&&) = delete;

		inline ComponentType GetComponentType() { return m_ComponentType; };
	private:
		ComponentType m_ComponentType;
	};

}