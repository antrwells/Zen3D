#include "pch.h"
#include "NodeLight.h"
#include "NodeCamera.h"
#include "SceneGraph.h"

	NodeLight::NodeLight(bool shadowFB) {

		mDiffuse = float3(1, 1, 1);
		mSpecular = float3(1, 1, 1);
		mAmbient = float3(0.2, 0.2, 0.2);
		mRange = 100;
		if (shadowFB) {

			//ShadowFB = new Kinetic::Framebuffer::FramebufferCube(ShadowMapRes, ShadowMapRes);
			mCastShadows = true;
		}
		else {
			//ShadowFB = nullptr;
			mCastShadows = false;

		}
		//mShadowCube = new CubeRenderer()
		mName = "LightSource";
		mType = NodeType::Light;
		mShadowCube = new RenderTargetCube(512, 512);
	}

	int NodeLight::ShadowMapRes = 1024;

	NodeLight* NodeLight::Active = nullptr;
