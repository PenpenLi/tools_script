//*****************************************************************************
//
//  Copyright (c) 2014-2014 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  PowerUp util
// 
//*****************************************************************************

#pragma once

class VuCarEntity;
class VuMatrix;
class VuJsonContainer;


namespace VuPowerUpUtil
{
	VUUINT32	createPfx(VuCarEntity *pCar, const char *pfxName, bool oneShot);
	VUUINT32	createPfx(VuCarEntity *pCar, const VuJsonContainer &data, bool oneShot);
	void		readTransform(const VuJsonContainer &data, VuMatrix &transform);

	void		createSfx(VuCarEntity *pCar, const char *sfxName);

	void		calcMissileLaunchTransform(VuCarEntity *pLauncher, VuMatrix &transform);
	VuCarEntity	*findBestMissileTarget(VuCarEntity *pLauncher, float range, float cosTargetAcquisitionCone);
}