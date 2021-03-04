#pragma once
#include "ECS/Components/CharacterComponent.h"
#include <algorithm>

struct Consideration
{
	Consideration() { m_Context = nullptr; };

	CharacterComponent* m_Context;

	virtual void SetContext(CharacterComponent& context)
	{
		m_Context = &context;
	}

	virtual float Evaluate()
	{
		return 0.0f;
	}
};

struct GoldConsideration : public Consideration
{
	GoldConsideration() : Consideration()
	{
	}

	void SetContext(CharacterComponent& context) override
	{
		m_Context = &context;
	}


	float Evaluate() override
	{
		float m_MaxEvalGold = 200;
		float m_EvalGold = m_Context->m_CurrentGold;

		if (m_Context->m_CurrentGold <= 0 || m_Context->m_CurrentArmySize == 0)
		{
			return 0.0;
		}
		
		else
		{
			if (m_EvalGold > m_MaxEvalGold)
			{
				return 1.0f;
			}

			else
			{
				//Todo: need to take army size into consideration

				//y = mx + c
				float eval = .5 * m_EvalGold + 0;
				return std::clamp(eval, 0.0f, 1.0f);
				
			}
		}
	}
};

struct ArmyConsideration : public Consideration
{
	ArmyConsideration() : Consideration() {};

	int m_MaxEvalSoldiers = 200;
	int m_CurrentArmySize = m_Context->m_CurrentArmySize;

	float Evaluate() override
	{
		if (m_Context->m_CurrentArmySize == 0) //Change later 
		{
			return 0.0;
		}

		else
		{
			if (m_CurrentArmySize > m_MaxEvalSoldiers)
			{
				return 1.0f;
			}

			else
			{
				//y = mx + c
				float eval = .5 * m_CurrentArmySize + 0;
				return std::clamp(eval, 0.0f, 1.0f);

			}
		}
	}
};
