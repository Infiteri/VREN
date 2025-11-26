#pragma once

#include "Base.h"
#include "Camera/OrthographicCamera.h"
#include <memory>

namespace VREN
{
    struct OrthoMovementFrameData
    {
        float NormalSpeed = 100.0f;
        float FastSpeed = 200.0f;
        float DeltaTime = 1.f / 60;
        float NormalZoom = 1;
        float FastZoom = 2;

        int ZoomScalar = 1;

        bool Up = false, Down = false, Left = false, Right = false;
        bool FastMove = false;
        bool UpdateZoom;
    };

    class VREN_API OrthoMovement
    {
    public:
        OrthoMovement(std::shared_ptr<OrthographicCamera> cam);
        ~OrthoMovement() = default;

        void Update(const OrthoMovementFrameData &data);

    private:
        std::shared_ptr<OrthographicCamera> camera;
    };
} // namespace VREN
