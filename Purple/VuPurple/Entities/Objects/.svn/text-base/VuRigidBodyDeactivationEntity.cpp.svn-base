//*****************************************************************************
//
//  Copyright (c) 2012-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  RigidBody deactivation entity
// 
//*****************************************************************************

#include "VuPurple/Dynamics/VuCollisionTypes.h"
#include "VuEngine/Entities/VuEntity.h"
#include "VuEngine/Components/Transform/VuTransformComponent.h"
#include "VuEngine/Components/3dLayout/Vu3dLayoutComponent.h"
#include "VuEngine/Components/RigidBody/VuRigidBodyComponent.h"
#include "VuEngine/Components/Script/VuScriptComponent.h"


class VuRigidBodyDeactivationEntity : public VuEntity
{
	DECLARE_RTTI

public:
	VuRigidBodyDeactivationEntity();

	virtual void		onPostLoad() { transformModified(); }
	virtual void		onGameInitialize();
	virtual void		onGameRelease();

private:
	// scripting
	VuRetVal			Enable(const VuParams &params)  { enable(); return VuRetVal(); }
	VuRetVal			Disable(const VuParams &params) { disable(); return VuRetVal(); }

	void				enable();
	void				disable();

	void				drawLayout(const Vu3dLayoutDrawParams &params);
	void				transformModified();

	// components
	VuScriptComponent		*mpScriptComponent;
	Vu3dLayoutComponent		*mp3dLayoutComponent;
	VuRigidBodyComponent	*mpRigidBodyComponent;

	// properties
	bool				mbInitiallyEnabled;
};


IMPLEMENT_RTTI(VuRigidBodyDeactivationEntity, VuEntity);
IMPLEMENT_ENTITY_REGISTRATION(VuRigidBodyDeactivationEntity);


//*****************************************************************************
VuRigidBodyDeactivationEntity::VuRigidBodyDeactivationEntity():
	mbInitiallyEnabled(true)
{
	// properties
	addProperty(new VuBoolProperty("Initially Enabled", mbInitiallyEnabled));

	// components
	addComponent(mpScriptComponent = new VuScriptComponent(this, 150, false));
	addComponent(mp3dLayoutComponent = new Vu3dLayoutComponent(this));
	addComponent(mpRigidBodyComponent = new VuRigidBodyComponent(this));

	mpTransformComponent->setWatcher(&VuRigidBodyDeactivationEntity::transformModified);
	mp3dLayoutComponent->setDrawMethod(this, &VuRigidBodyDeactivationEntity::drawLayout);

	// scripting
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRigidBodyDeactivationEntity, Enable);
	ADD_SCRIPT_INPUT_NOARGS(mpScriptComponent, VuRigidBodyDeactivationEntity, Disable);
}

//*****************************************************************************
void VuRigidBodyDeactivationEntity::onGameInitialize()
{
	mpRigidBodyComponent->setCollisionGroup(COL_GAME_DEACTIVATION);
	mpRigidBodyComponent->setCollisionMask(COL_ENGINE_DYNAMIC_PROP);
	mpRigidBodyComponent->setCollisionFlags(mpRigidBodyComponent->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	mpRigidBodyComponent->createRigidBody();

	if ( mbInitiallyEnabled )
		enable();
}

//*****************************************************************************
void VuRigidBodyDeactivationEntity::onGameRelease()
{
	disable();

	mpRigidBodyComponent->destroyRigidBody();
}

//*****************************************************************************
void VuRigidBodyDeactivationEntity::enable()
{
	mpRigidBodyComponent->addToWorld();
}

//*****************************************************************************
void VuRigidBodyDeactivationEntity::disable()
{
	mpRigidBodyComponent->removeFromWorld();
}

//*****************************************************************************
void VuRigidBodyDeactivationEntity::drawLayout(const Vu3dLayoutDrawParams &params)
{
	if ( params.mbDrawCollision )
	{
		mpRigidBodyComponent->draw(VuColor(255,255,255), params.mCamera);
	}
}

//*****************************************************************************
void VuRigidBodyDeactivationEntity::transformModified()
{
	VuMatrix mat = mpTransformComponent->getWorldTransform();
	mat.scaleLocal(mpTransformComponent->getWorldScale());

	mpRigidBodyComponent->transformModified(mpTransformComponent->getWorldTransform());
	mpRigidBodyComponent->scaleModified(mpTransformComponent->getWorldScale());
}
