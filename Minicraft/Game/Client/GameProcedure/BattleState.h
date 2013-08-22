/********************************************************************
	filename	BattleState.h
	author:		maval

	purpose:	��Ϸ������״̬
*********************************************************************/

#ifndef BATTLE_STATE_H
#define BATTLE_STATE_H

#include "AppState.h"
#include "InputManager.h"
#include "KratosPrerequisites.h"

class CBattleState : public Kratos::CAppState
{
public:
	CBattleState();
	~CBattleState();

	DECLARE_APPSTATE_CLASS(CBattleState)

	void enter();
	void createScene();
	void exit();
	bool pause();
	void resume();
	void update(float timeSinceLastFrame);

	static std::string StateName;

private:
	bool OnInputSys_MousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool OnInputSys_MouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	bool OnInputSys_MouseMove(const OIS::MouseEvent& arg);
	bool OnInputSys_KeyPressed(const OIS::KeyEvent& arg);
	bool OnInputSys_KeyReleased(const OIS::KeyEvent& arg);

private:
	bool	m_bQuit;
	bool	m_bCamMoveLeft, m_bCamMoveRight, m_bCamMoveUp, m_bCamMoveDown;
	bool	m_bLBDown;		//�������Ƿ���
	Ogre::Vector3	m_LBDownPos;	//���������µĶ�Ӧ��������
	Ogre::Vector2	m_LBDownScreenPos;
	Ogre::Rectangle2D*	m_pSelectionQuad;	//��ѡ����
	Ogre::SceneNode*	m_pQuadNode;
};


#endif