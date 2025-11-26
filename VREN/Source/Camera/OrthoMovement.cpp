#include "OrthoMovement.h"

namespace VREN
{
    OrthoMovement::OrthoMovement(std::shared_ptr<OrthographicCamera> cam) : camera(cam) {}

    void OrthoMovement::Update(const OrthoMovementFrameData &data)
    {
        bool updateView = false;
        float speed = data.FastMove ? data.FastSpeed : data.NormalSpeed;

        if (data.Up)
        {
            camera->GetPosition().y -= speed * data.DeltaTime;
            updateView = true;
        }

        if (data.Down)
        {
            camera->GetPosition().y += speed * data.DeltaTime;
            updateView = true;
        }

        if (data.Left)
        {
            camera->GetPosition().x -= speed * data.DeltaTime;
            updateView = true;
        }

        if (data.Right)
        {
            camera->GetPosition().x += speed * data.DeltaTime;
            updateView = true;
        }

        if (data.UpdateZoom)
        {
            float zSpeed = data.FastMove ? data.FastZoom : data.NormalZoom;
            camera->SetZoom(camera->Zoom() + zSpeed * data.ZoomScalar, true);
        }

        if (updateView)
            camera->UpdateView();
    }
} // namespace VREN
