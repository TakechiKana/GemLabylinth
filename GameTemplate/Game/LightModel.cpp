#include "stdafx.h"
#include "LightModel.h"

LightModel::LightModel()
{
	m_modelRender.Init("Assets/modelData/stage/light.tkm");
	m_modelRender.SetShadowCasterFlag(false);

	m_modelRender.Update();
}

LightModel::~LightModel()
{

}

void LightModel::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}