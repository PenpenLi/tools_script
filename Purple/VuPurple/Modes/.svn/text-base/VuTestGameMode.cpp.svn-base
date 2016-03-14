//*****************************************************************************
//
//  Copyright (c) 2011-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  FrontEndGameMode class
// 
//*****************************************************************************

#include "VuTestGameMode.h"
#include "VuPurple/Managers/VuFadeManager.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Gfx/Shaders/VuBasicShaders.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/Gfx/VuShaderProgram.h"
#include "VuEngine/HAL/Gfx/VuVertexDeclaration.h"
#include "VuEngine/HAL/Gfx/VuPipelineState.h"
#include "VuEngine/HAL/Gfx/VuVertexBuffer.h"
#include "VuEngine/HAL/Gfx/VuIndexBuffer.h"
#include "VuEngine/HAL/Gfx/VuTexture.h"
#include "VuEngine/HAL/Gfx/VuRenderTarget.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuCompiledShaderAsset.h"
#include "VuEngine/Assets/VuTextureAsset.h"
#include "VuEngine/Math/VuMatrix.h"


//*****************************************************************************
VuTestGameMode::VuTestGameMode()
{
}

//*****************************************************************************
VuTestGameMode::~VuTestGameMode()
{
}

//*****************************************************************************
bool VuTestGameMode::enter(const std::string &prevMode)
{
	// build verts
	float extent = VuSqrt(2.0f)/4.0f;
	mVerts[0].mXyz[0] = -extent; mVerts[0].mXyz[1] = -extent; mVerts[0].mXyz[2] = 0.0f; mVerts[0].mUv[0] = 0.0f; mVerts[0].mUv[1] = 0.0f;
	mVerts[1].mXyz[0] = -extent; mVerts[1].mXyz[1] =  extent; mVerts[1].mXyz[2] = 0.0f; mVerts[1].mUv[0] = 0.0f; mVerts[1].mUv[1] = 1.0f;
	mVerts[2].mXyz[0] =  extent; mVerts[2].mXyz[1] = -extent; mVerts[2].mXyz[2] = 0.0f; mVerts[2].mUv[0] = 1.0f; mVerts[2].mUv[1] = 0.0f;
	mVerts[3].mXyz[0] =  extent; mVerts[3].mXyz[1] =  extent; mVerts[3].mXyz[2] = 0.0f; mVerts[3].mUv[0] = 1.0f; mVerts[3].mUv[1] = 1.0f;

	// build inds
	mInds[0] = 0;
	mInds[1] = 1;
	mInds[2] = 2;
	mInds[3] = 3;

	// create mesh using vertex/index buffer
	{
		// create vertex buffer
		VuVertexBuffer *pVertexBuffer = VuGfx::IF()->createVertexBuffer(4*sizeof(Vertex2dXyzUv), 0);
		if ( !pVertexBuffer )
			return VUWARNING("Error creating vertex buffer.\n");
		pVertexBuffer->setData(mVerts, sizeof(mVerts));

		// create index buffer
		VuIndexBuffer *pIndexBuffer = VuGfx::IF()->createIndexBuffer(4, 0);
		if ( !pIndexBuffer )
			return VUWARNING("Error creating index buffer.\n");
		pIndexBuffer->setData(mInds, 4);

		VuGfxSortMeshDesc desc;
		desc.mpVertexBuffer = pVertexBuffer;
		desc.mpIndexBuffer = pIndexBuffer;
		mpGfxSortMesh = VuGfxSort::IF()->createMesh(desc);

		pVertexBuffer->removeRef();
		pIndexBuffer->removeRef();
	}

	// load texture
	{
		mpTextureAsset = VuAssetFactory::IF()->createAsset<VuTextureAsset>("Pfx/Chicken");
		if ( !mpTextureAsset )
			return VUWARNING("Error loading texture\n");
	}

	VuFadeManager::IF()->startFadeIn();

	return true;
}

//*****************************************************************************
void VuTestGameMode::exit()
{
	VuGfxSort::IF()->releaseMesh(mpGfxSortMesh);
	VuAssetFactory::IF()->releaseAsset(mpTextureAsset);
}

//*****************************************************************************
const char *VuTestGameMode::tick(float fdt)
{
	return "";
}

//*****************************************************************************
void VuTestGameMode::draw()
{
	VuGfxSort::IF()->setFullScreenLayer(0);
	VuGfxSort::IF()->setViewport(0);
	VuGfxSort::IF()->setReflectionLayer(0);
	VuGfxSort::IF()->setViewportLayer(0);

	VuSetRenderTargetParams params(VUNULL);
	params.mColorLoadAction = VuSetRenderTargetParams::LoadActionClear;
	params.mDepthLoadAction = VuSetRenderTargetParams::LoadActionClear;
	VuGfxUtil::IF()->submitSetRenderTargetCommand(params);

	VuGfxSort::IF()->setFullScreenLayer(VuGfxSort::FSL_UI);
	VuGfxSort::IF()->setViewport(0);
	VuGfxSort::IF()->setReflectionLayer(VuGfxSort::REFLECTION_OFF);
	VuGfxSort::IF()->setViewportLayer(VuGfxSort::VPL_UI);

	VuTestGameMode **ppTest = static_cast<VuTestGameMode **>(VuGfxSort::IF()->allocateCommandMemory(sizeof(VuTestGameMode *)));
	*ppTest = this;

	VuGfxSortMaterial *pMaterial = VuGfxUtil::IF()->basicShaders()->get2dXyzUvMaterial(VuBasicShaders::FLV_MODULATED);
	VuGfxSort::IF()->submitDrawCommand<false>(VuGfxSort::TRANS_UI_MODULATE, pMaterial, mpGfxSortMesh, &VuTestGameMode::staticDrawCallback);
}

//*****************************************************************************
void VuTestGameMode::drawCallback()
{
	// render state
	VuGfx::IF()->setCullMode(VUGFX_CULL_CW);

	int width, height;
	VuGfx::IF()->getDisplaySize(VUNULL, width, height);
	float aspectRatio = (float)width/height;

	VuMatrix transform;
	transform.loadIdentity();
	transform.rotateZ(-(float)VuSys::IF()->getTime());
	transform.scale(VuVector3(1.0f/aspectRatio, 1.0f, 1.0f));
	transform.translate(VuVector3(0.5f, 0.5f, 0.0f));

	VuGfxUtil::IF()->basicShaders()->set2dXyzUvConstants(transform, VuColor(255,255,255));
	
	VuGfx::IF()->setTexture(0, mpTextureAsset->getTexture());
	//VuGfx::IF()->setTexture(0, VuGfxUtil::IF()->whiteTexture());

	//VuGfx::IF()->drawPrimitiveUP(VUGFX_PT_TRIANGLESTRIP, 1, mVerts);
	//VuGfx::IF()->drawIndexedPrimitiveUP(VUGFX_PT_TRIANGLESTRIP, 0, 3, 1, mInds, mVerts);
	//VuGfx::IF()->drawPrimitive(VUGFX_PT_TRIANGLESTRIP, 0, 1);
	VuGfx::IF()->drawIndexedPrimitive(VUGFX_PT_TRIANGLESTRIP, 0, 4, 0, 2);
	
	//transform.scaleLocal(VuVector3(0.5f, 0.5f, 1.0f));
	//transform.rotateZLocal(VU_PIDIV2);
	//VuGfxUtil::IF()->basicShaders()->set2dXyzUvConstants(transform, VuColor(255,128,128));
	//VuGfx::IF()->drawIndexedPrimitive(VUGFX_PT_TRIANGLESTRIP, 0, 4, 0, 2);
}