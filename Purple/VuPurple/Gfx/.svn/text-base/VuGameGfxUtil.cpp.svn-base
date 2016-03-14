//*****************************************************************************
//
//  Copyright (c) 2013-2013 Vector Unit Inc
//  Confidential Trade Secrets
// 
//  Game Gfx util
// 
//*****************************************************************************

#include "VuGameGfxUtil.h"
#include "VuEngine/Assets/VuAssetFactory.h"
#include "VuEngine/Assets/VuCompiledShaderAsset.h"
#include "VuEngine/Gfx/VuGfxUtil.h"
#include "VuEngine/Gfx/GfxSort/VuGfxSort.h"
#include "VuEngine/Gfx/Shaders/VuBasicShaders.h"
#include "VuEngine/HAL/Gfx/VuGfx.h"
#include "VuEngine/HAL/Gfx/VuVertexDeclaration.h"
#include "VuEngine/HAL/Gfx/VuTexture.h"
#include "VuEngine/HAL/Gfx/VuShaderProgram.h"
#include "VuEngine/HAL/Gfx/VuPipelineState.h"
#include "VuEngine/Math/VuRand.h"
#include "VuEngine/Memory/VuScratchPad.h"


// the interface
IMPLEMENT_SYSTEM_COMPONENT(VuGameGfxUtil, VuGameGfxUtil);


//*****************************************************************************
VuGameGfxUtil::VuGameGfxUtil():
	mpCarIconMaterial(VUNULL),
	mpGrayScaleMaterial(VUNULL)
{
}

//*****************************************************************************
VuGameGfxUtil::~VuGameGfxUtil()
{
}

//*****************************************************************************
bool VuGameGfxUtil::init()
{
	if ( VuAssetFactory::IF() && VuGfx::IF() )
	{
		// create materials
		VuVertexDeclarationParams vdParams;
		vdParams.mElements.push_back(VuVertexDeclarationElement(0,  0, VUGFX_DECL_TYPE_FLOAT3, VUGFX_DECL_USAGE_POSITION, 0));
		vdParams.mElements.push_back(VuVertexDeclarationElement(0, 12, VUGFX_DECL_TYPE_FLOAT2, VUGFX_DECL_USAGE_TEXCOORD, 0));
		vdParams.mStreams.push_back(VuVertexDeclarationStream(20));

		{
			VuCompiledShaderAsset *pShaderAsset = VuAssetFactory::IF()->createAsset<VuCompiledShaderAsset>("UI_CarIcon");

			VuVertexDeclaration *pVD = VuGfx::IF()->createVertexDeclaration(vdParams, pShaderAsset->getShaderProgram());

			VuPipelineStateParams psParams;
			psParams.mAlphaBlendEnabled = true;
			psParams.mSrcBlendMode = VUGFX_BLEND_SRCALPHA;
			psParams.mDstBlendMode = VUGFX_BLEND_INVSRCALPHA;
			VuPipelineState *pPS = VuGfx::IF()->createPipelineState(pShaderAsset->getShaderProgram(), pVD, psParams);

			VuGfxSortMaterialDesc desc;
			mpCarIconMaterial = VuGfxSort::IF()->createMaterial(pPS, desc);

			VuAssetFactory::IF()->releaseAsset(pShaderAsset);
			pVD->removeRef();
			pPS->removeRef();
		}

		{
			VuCompiledShaderAsset *pShaderAsset = VuAssetFactory::IF()->createAsset<VuCompiledShaderAsset>("UI_GrayScale");

			VuVertexDeclaration *pVD = VuGfx::IF()->createVertexDeclaration(vdParams, pShaderAsset->getShaderProgram());

			VuPipelineStateParams psParams;
			psParams.mAlphaBlendEnabled = true;
			psParams.mSrcBlendMode = VUGFX_BLEND_SRCALPHA;
			psParams.mDstBlendMode = VUGFX_BLEND_INVSRCALPHA;
			VuPipelineState *pPS = VuGfx::IF()->createPipelineState(pShaderAsset->getShaderProgram(), pVD, psParams);

			VuGfxSortMaterialDesc desc;
			mpGrayScaleMaterial = VuGfxSort::IF()->createMaterial(pPS, desc);

			VuAssetFactory::IF()->releaseAsset(pShaderAsset);
			pVD->removeRef();
			pPS->removeRef();
		}
	}

	return true;
}

//*****************************************************************************
void VuGameGfxUtil::release()
{
	VuGfxSort::IF()->releaseMaterial(mpCarIconMaterial);
	VuGfxSort::IF()->releaseMaterial(mpGrayScaleMaterial);
}

//*****************************************************************************
void VuGameGfxUtil::drawLightning(const LightningParams &params)
{
	struct DrawData
	{
		static void callback(void *data)
		{
			DrawData *pData = static_cast<DrawData *>(data);
			pData->draw();
		}

		void draw()
		{
			VuGfxUtil::IF()->basicShaders()->set3dXyzConstants(mParams.mViewProjMat, mParams.mColor);

			VuGfx::IF()->setTexture(0, VUNULL);

			VuRand rand(mParams.mRandomSeed);

			int lineCount = (1<<(mParams.mDetail - 1));
			int vertCount = lineCount + 1;

			int totalSize = 0;
			totalSize += vertCount*sizeof(VuVector3);
			totalSize += 2*vertCount*sizeof(VuVertex3dXyz);
			VUASSERT(totalSize <= VuScratchPad::SIZE, "VuLightningEntity::draw() exceeded scratch pad size");

			float extent = 0.5f*mParams.mWidth;

			for ( int iBolt = 0; iBolt < mParams.mBoltCount; iBolt++ )
			{
				// recursively build verts to scratchpad
				VuVector3 *pVerts = (VuVector3 *)VuScratchPad::get(VuScratchPad::GRAPHICS);

				int i0 = 0;
				int i1 = vertCount - 1;

				pVerts[i0] = mParams.mSrcPos;
				pVerts[i1] = mParams.mDstPos;

				drawLightning(pVerts, i0, i1, mParams.mDisplacement, rand);

				// now build trail verts
				VuVertex3dXyz *pTrailVerts = (VuVertex3dXyz *)(pVerts + vertCount);
				VuVertex3dXyz *pTrailVert = pTrailVerts;
				VuVector3 *pVert = pVerts;

				// first pos
				VuVector3 pos = pVert[0];
				VuVector3 nextPos = pVert[1];
				VuVector3 axis = VuCross(nextPos - pos, pos - mParams.mEyePosition).normal()*extent;

				pTrailVert->mXyz[0] = pos.mX + axis.mX; pTrailVert->mXyz[1] = pos.mY + axis.mY; pTrailVert->mXyz[2] = pos.mZ + axis.mZ;
				pTrailVert++;
				pTrailVert->mXyz[0] = pos.mX - axis.mX; pTrailVert->mXyz[1] = pos.mY - axis.mY; pTrailVert->mXyz[2] = pos.mZ - axis.mZ;
				pTrailVert++;
				pVert++;

				VuVector3 lastPos;
				for ( int i = 1; i < vertCount - 1; i++ )
				{
					lastPos = pos;
					pos = nextPos;
					nextPos = pVert[1];
					axis = VuCross(nextPos - lastPos, pos - mParams.mEyePosition).normal()*extent;

					pTrailVert->mXyz[0] = pos.mX + axis.mX; pTrailVert->mXyz[1] = pos.mY + axis.mY; pTrailVert->mXyz[2] = pos.mZ + axis.mZ;
					pTrailVert++;
					pTrailVert->mXyz[0] = pos.mX - axis.mX; pTrailVert->mXyz[1] = pos.mY - axis.mY; pTrailVert->mXyz[2] = pos.mZ - axis.mZ;
					pTrailVert++;
					pVert++;
				}

				// last pos
				lastPos = pos;
				pos = pVert[0];
				axis = VuCross(pos - lastPos, pos - mParams.mEyePosition).normal()*extent;

				pTrailVert->mXyz[0] = pos.mX + axis.mX; pTrailVert->mXyz[1] = pos.mY + axis.mY; pTrailVert->mXyz[2] = pos.mZ + axis.mZ;
				pTrailVert++;
				pTrailVert->mXyz[0] = pos.mX - axis.mX; pTrailVert->mXyz[1] = pos.mY - axis.mY; pTrailVert->mXyz[2] = pos.mZ - axis.mZ;
				pTrailVert++;
				pVert++;

				VuGfx::IF()->drawPrimitiveUP(VUGFX_PT_TRIANGLESTRIP, (vertCount - 1)*2, pTrailVerts);
			}
		}

		static void drawLightning(VuVector3 *pVerts, int i0, int i1, float displacement, VuRand &rand)
		{
			int im = (i0 + i1)/2;
			VuVector3 &v0 = pVerts[i0];
			VuVector3 &v1 = pVerts[i1];
			VuVector3 &vm = pVerts[im];

			vm.mX = 0.5f*(v0.mX + v1.mX) + (rand.rand() - 0.5f)*displacement;
			vm.mY = 0.5f*(v0.mY + v1.mY) + (rand.rand() - 0.5f)*displacement;
			vm.mZ = 0.5f*(v0.mZ + v1.mZ) + (rand.rand() - 0.5f)*displacement;

			if ( im - i0 > 1 )
			{
				drawLightning(pVerts, i0, im, 0.5f*displacement, rand);
				drawLightning(pVerts, im, i1, 0.5f*displacement, rand);
			}
		}

		LightningParams	mParams;
	};

	DrawData *pData = static_cast<DrawData *>(VuGfxSort::IF()->allocateCommandMemory(sizeof(DrawData)));
	pData->mParams = params;

	VuGfxSort::IF()->submitDrawCommand<false>(VuGfxSort::TRANS_OPAQUE, VuGfxUtil::IF()->basicShaders()->get3dXyzMaterial(VuBasicShaders::FLV_OPAQUE), VUNULL, DrawData::callback);
}

//*****************************************************************************
void VuGameGfxUtil::drawCarIcon(const CarIconDrawParams &params)
{
	struct DrawData
	{
		static void callback(void *data)
		{
			DrawData *pData = static_cast<DrawData *>(data);

			VuShaderProgram *pSP = VuGameGfxUtil::IF()->mpCarIconMaterial->mpShaderProgram;

			pSP->setConstantMatrix(pSP->getConstantByName("gTransform"), pData->mTransform);
			pSP->setConstantColor4(pSP->getConstantByName("gColor"), pData->mColor);

			pSP->setConstantColor3(pSP->getConstantByName("PaintColor"), pData->mPaintColor);

			VuGfx::IF()->setTexture(pSP->getSamplerIndexByName("IconTexture"), pData->mpIconTexture);
			VuGfx::IF()->setTexture(pSP->getSamplerIndexByName("MaskTexture"), pData->mpMaskTexture);

			float fx0 = pData->mDstRect.getLeft();
			float fx1 = pData->mDstRect.getRight();
			float fy0 = pData->mDstRect.getTop();
			float fy1 = pData->mDstRect.getBottom();

			float fu0 = pData->mSrcRect.getLeft();
			float fu1 = pData->mSrcRect.getRight();
			float fv0 = pData->mSrcRect.getTop();
			float fv1 = pData->mSrcRect.getBottom();

			VuVertex2dXyzUv verts[4];
			verts[0].mXyz[0] = fx0; verts[0].mXyz[1] = fy0; verts[0].mXyz[2] = pData->mDepth; verts[0].mUv[0] = fu0; verts[0].mUv[1] = fv0;
			verts[1].mXyz[0] = fx0; verts[1].mXyz[1] = fy1; verts[1].mXyz[2] = pData->mDepth; verts[1].mUv[0] = fu0; verts[1].mUv[1] = fv1;
			verts[2].mXyz[0] = fx1; verts[2].mXyz[1] = fy0; verts[2].mXyz[2] = pData->mDepth; verts[2].mUv[0] = fu1; verts[2].mUv[1] = fv0;
			verts[3].mXyz[0] = fx1; verts[3].mXyz[1] = fy1; verts[3].mXyz[2] = pData->mDepth; verts[3].mUv[0] = fu1; verts[3].mUv[1] = fv1;
			
			VuGfx::IF()->drawPrimitiveUP(VUGFX_PT_TRIANGLESTRIP, 2, verts);
		}

		VuMatrix		mTransform;
		float			mDepth;
		VuRect			mDstRect;
		VuRect			mSrcRect;
		VuTexture		*mpIconTexture;
		VuTexture		*mpMaskTexture;
		VuColor			mColor;
		VuColor			mPaintColor;
	};

	DrawData *pData = static_cast<DrawData *>(VuGfxSort::IF()->allocateCommandMemory(sizeof(DrawData)));
	pData->mTransform = VuGfxUtil::IF()->getMatrix();
	pData->mDepth = params.mDepth;
	pData->mpIconTexture = params.mpIconTexture;
	pData->mpMaskTexture = params.mpMaskTexture;
	pData->mDstRect = params.mDstRect;
	pData->mSrcRect = params.mSrcRect;
	pData->mColor = params.mColor;
	pData->mPaintColor = params.mPaintColor;

	VuGfxSort::IF()->submitDrawCommand<true>(VuGfxSort::TRANS_UI_MODULATE, mpCarIconMaterial, VUNULL, &DrawData::callback, params.mDepth);
}

//*****************************************************************************
void VuGameGfxUtil::drawGrayScaleTexture2d(float depth, VuTexture *pTexture, const VuColor &color, const VuRect &dstRect, const VuRect &srcRect)
{
	struct DrawData
	{
		static void callback(void *data)
		{
			DrawData *pData = static_cast<DrawData *>(data);

			VuShaderProgram *pSP = VuGameGfxUtil::IF()->mpGrayScaleMaterial->mpShaderProgram;

			pSP->setConstantMatrix(pSP->getConstantByName("gTransform"), pData->mTransform);
			pSP->setConstantColor4(pSP->getConstantByName("gColor"), pData->mColor);

			VuGfx::IF()->setTexture(pSP->getSamplerIndexByName("tex0"), pData->mpTexture);

			float fx0 = pData->mDstRect.getLeft();
			float fx1 = pData->mDstRect.getRight();
			float fy0 = pData->mDstRect.getTop();
			float fy1 = pData->mDstRect.getBottom();

			float fu0 = pData->mSrcRect.getLeft();
			float fu1 = pData->mSrcRect.getRight();
			float fv0 = pData->mSrcRect.getTop();
			float fv1 = pData->mSrcRect.getBottom();

			VuVertex2dXyzUv verts[4];
			verts[0].mXyz[0] = fx0; verts[0].mXyz[1] = fy0; verts[0].mXyz[2] = pData->mDepth; verts[0].mUv[0] = fu0; verts[0].mUv[1] = fv0;
			verts[1].mXyz[0] = fx0; verts[1].mXyz[1] = fy1; verts[1].mXyz[2] = pData->mDepth; verts[1].mUv[0] = fu0; verts[1].mUv[1] = fv1;
			verts[2].mXyz[0] = fx1; verts[2].mXyz[1] = fy0; verts[2].mXyz[2] = pData->mDepth; verts[2].mUv[0] = fu1; verts[2].mUv[1] = fv0;
			verts[3].mXyz[0] = fx1; verts[3].mXyz[1] = fy1; verts[3].mXyz[2] = pData->mDepth; verts[3].mUv[0] = fu1; verts[3].mUv[1] = fv1;
			
			VuGfx::IF()->drawPrimitiveUP(VUGFX_PT_TRIANGLESTRIP, 2, verts);
		}

		float		mDepth;
		VuTexture	*mpTexture;
		VuMatrix	mTransform;
		VuColor		mColor;
		VuRect		mSrcRect;
		VuRect		mDstRect;
	};

	DrawData *pData = static_cast<DrawData *>(VuGfxSort::IF()->allocateCommandMemory(sizeof(DrawData)));
	pData->mDepth = depth;
	pData->mpTexture = pTexture;
	pData->mTransform = VuGfxUtil::IF()->getMatrix();
	pData->mColor = color;
	pData->mSrcRect = srcRect;
	pData->mDstRect = dstRect;

	VuGfxSort::IF()->submitDrawCommand<true>(VuGfxSort::TRANS_UI_MODULATE, mpGrayScaleMaterial, VUNULL, &DrawData::callback, depth);
}
