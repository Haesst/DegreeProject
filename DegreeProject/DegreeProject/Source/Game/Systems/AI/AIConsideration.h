#pragma once
#include "ECS/Components/CharacterComponent.h"
#include <algorithm>

struct Consideration
{
	CharacterComponent* m_Context;

	Consideration() { m_Context = nullptr; };


	virtual void SetContext(CharacterComponent& context)
	{
		m_Context = &context;
	}

	//context = Who is making the decision, target is optional if we need to compare values with another character
	virtual float Evaluate(CharacterComponent* context, CharacterComponent* target = nullptr)
	{
		return 0.0f;
	}
};

//The smaller army size of the enemy, the greater chance of winning the war
struct EnemyArmyConsideration : public Consideration
{
	EnemyArmyConsideration() : Consideration()
	{

	}

	void SetContext(CharacterComponent& context) override
	{
		m_Context = &context;
	}

	float Evaluate(CharacterComponent* context, CharacterComponent* target = nullptr) override
	{
		if (target == nullptr)
		{
			return 0.0f;
		}

		float armySizeDiff = context->m_CurrentArmySize - target->m_CurrentArmySize;
		std::abs(armySizeDiff);


		//y = mx + c
		float eval = -.1 * armySizeDiff + 1;
		return std::clamp(eval, 0.0f, 1.0f);
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


	float Evaluate(CharacterComponent* context, CharacterComponent* target = nullptr) override
	{
		if (target == nullptr)
		{
			return 0.0f;
		}

		float targetGold = target->m_CurrentGold;
		float m_EvalGold = m_Context->m_CurrentGold;

		float evalValue = targetGold / m_EvalGold;

		//y = mx + c
		float eval = .5 * m_EvalGold + 0;
		return std::clamp(eval, 0.0f, 1.0f);

	}
};

struct ArmyConsideration : public Consideration
{
	ArmyConsideration() : Consideration() {};

	float Evaluate(CharacterComponent* context, CharacterComponent* target = nullptr) override
	{
		int m_MaxEvalSoldiers = 200;
		int m_CurrentArmySize = m_Context->m_CurrentArmySize;

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
