/********************************************************************
	created:	23:1:2013   1:06
	filename: 	F:\MiniCraft\MiniCraft\Game\Client\Selectable.h
	author:		maval
	
	purpose:	��ѡ���������.��ѡ�������ܹ�ӵ�м���Command.
*********************************************************************/

#ifndef SelectableObject_h__
#define SelectableObject_h__

#include "ObjectBase.h"
#include "GameDefine.h"
#include "Command.h"

struct SAbilityData;
class AiComponent;
class PathComponent;
class AnimatedComponent;
class HarvestComponent;
class BehaviorComponent;
class RagdollComponent;

///����ʵ������OgreProcedural��
class SelectableObject : public RenderableObject
{
public:
	SelectableObject();
	~SelectableObject();

public:
	virtual void	AddComponent(eComponentType type, Component* pCo);
	virtual void	CreateRenderInstance();
	virtual void	InitTeamColor(const COLOR& color);
	//��ǰ�������,�ӵ�֪ͨ
	virtual void	_OnCommandFinished(eCommandType cmd);

public:
	//!!��Ogre Root����ǰ�������
	static void		ReleaseMeshCache();

	void			SetSelected(bool bSelected);
	bool			GetSelected() const { return m_bSelected; }
	void			SetAbility(int slotIndex, const SAbilityData* pData);
	void			SetActiveAbility(int slotIndex) { m_pActiveAbility = m_pAbilitySlots[slotIndex]; } 
	const SAbilityData*	GetActiveAbility() const { return m_pActiveAbility; }
	bool			HasAbility(eCommandType type);

	AiComponent*			GetAi()	;		
	PathComponent*			GetPath();	
	AnimatedComponent*		GetAnim();		
	HarvestComponent*		GetGather();	
	BehaviorComponent*		GetBehavior();
	RagdollComponent*		GetRagdoll();

protected:
	virtual void	_OnSelected(bool bSelected);
	bool			m_bSelected;

	AiComponent*			m_pAi;
	PathComponent*			m_pPath;
	AnimatedComponent*		m_pAnim;
	HarvestComponent*		m_pGather;
	BehaviorComponent*		m_pBehavior;
	RagdollComponent*		m_pRagdoll;
	
private:
	Ogre::MeshPtr	_CreateSelectionCircleMesh(const Ogre::MeshPtr& objMesh);

	typedef std::unordered_map<STRING, Ogre::MeshPtr>	SelectionCircleCache;
	static SelectionCircleCache	m_selCircleCache;	//cacheÿ�ֵ�λ��ѡ�п�mesh
	Ogre::SceneNode*			m_pSelCircleNode;

	SAbilityData*				m_pAbilitySlots[MAX_ABILITY_SLOT];
	const SAbilityData*			m_pActiveAbility;
};

#endif // SelectableObject_h__