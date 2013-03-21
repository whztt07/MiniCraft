/********************************************************************
	created:	11:3:2013   17:07
	filename	AnimatedComponent.h
	author:		maval

	purpose:	�����������.
				note:Ŀǰ��ֻ��Unit�����ܹ�ӵ�и����
*********************************************************************/

#ifndef AnimatedComponent_h__
#define AnimatedComponent_h__

#include "Component.h"
#include "GameDataDef.h"
#include "KratosPrerequisites.h"

class SelectableObject;

class AnimatedComponent : public Component
{
public:
	AnimatedComponent(SelectableObject* pOwner);
	~AnimatedComponent();

public:
	virtual void	Update(float dt);

public:
	void PlayAnimation(eAnimation type, bool bLoop);
	void StopAnimation();

private:
	Kratos::EffectController*	m_pController;
};

#endif // AnimatedComponent_h__