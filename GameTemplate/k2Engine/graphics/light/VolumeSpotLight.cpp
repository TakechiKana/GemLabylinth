#include "k2EnginePreCompile.h"
#include "VolumeSpotLight.h"

namespace nsK2Engine {
	void VolumeSpotLight::Update()
	{
		// スポットライトの情報をもとにワールド行列を更新する。
		Vector3 pos = m_lightData->GetPosition();
		Quaternion rot;
		rot.SetRotation(g_vec3Back, m_lightData->GetDirection());
		
		Vector3 scale;
		scale.z = m_lightData->GetRange();
		float angle = m_lightData->GetAngle();
		/*angle = max(angle, m_lightData->GetAngle2());
		angle = max(angle, m_lightData->GetAngle3());*/
		// 角度からxyの拡大率を計算する。
		float xyScale = tan(angle) * m_lightData->GetRange() ;
		scale.x = xyScale;
		scale.y = xyScale;
		m_modelBack.UpdateWorldMatrix(pos, rot, scale);
		m_modelFront.UpdateWorldMatrix(pos, rot, scale);
	}
}