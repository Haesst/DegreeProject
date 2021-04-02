#include "AI.h"

//void AISystem::update()
//{
//	m_Characters = m_EntityManager->getComponentArray<CharacterComponent>();
//	//m_Warminds = m_EntityManager->getComponentArray<WarmindComponent>();
//	m_TickAccu += Time::deltaTime();
//
//	if (m_TickAccu <= m_AIUpdateTickRate)
//	{
//		return;
//	}
//
//	//AI System update considerations
//	for (auto entity : m_Entities)
//	{
//		if (expansionDecision(entity) > .8f) //Add personality weight
//		{
//			if (warDecision(entity) > .7f)
//			{
//				if (!m_Characters[entity].m_AtWar)
//				{
//#pragma warning(push)
//#pragma warning(disable: 26815)
//					CharacterSystem* characterSystem = (CharacterSystem*)m_EntityManager->getSystem<CharacterSystem>().get();
//#pragma warning(pop)
//					characterSystem->declareWar(m_Characters[entity].getID(), m_Warminds[entity].m_Opponent, m_Warminds[entity].m_WargoalRegionId);
//				}
//			}
//		}
//
//		m_TickAccu = 0.0f;
//	}
//}
//
//float AISystem::warDecision(EntityID ent)
//{
//	return 0.0f;
//}
//
//float AISystem::expansionDecision(EntityID ent)
//{
//	return 0.0f;
//}