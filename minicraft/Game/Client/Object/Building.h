/********************************************************************
	created:	24:1:2013   0:34
	filename: 	F:\MiniCraft\MiniCraft\Game\Client\Object\Building.h
	author:		maval
	
	purpose:	������
*********************************************************************/

#ifndef Building_h__
#define Building_h__

#include "SelectableObject.h"

class Building : public SelectableObject
{
public:
	Building();
	~Building() {}

public:
	virtual eObjectType GetType() const { return eObjectType_Building; }
	virtual void	Update(float dt) {}

public:
	void			SetBuildingName(const STRING& name) { m_buildingName = name; }

private:
	STRING			m_buildingName;
};


#endif // Building_h__