#pragma once
#undef near
#undef far

#include "Base.h"
#include "Camera.h"

namespace VREN
{
    class VREN_API PerspectiveCamera : public Camera
    {
    public:
        PerspectiveCamera();
        PerspectiveCamera(float fov, float aspect, float near, float far);

        void UpdateProjection();
        void UpdateProjection(float aspect);
        void UpdateProjection(float fov, float aspect, float near, float far);

        inline float GetFOV() { return fov; };
        inline float GetAspect() { return fov; };
        inline float GetNear() { return fov; };
        inline float GetFar() { return fov; };

        void SetFOV(const float fov, bool recalculateProjection = true);

        void SetAspect(const float aspect, bool recalculateProjection = true);
        void SetAspect(float width, float height, bool updateProjection);

        void SetNear(const float near, bool recalculateProjection = true);

        void SetFar(const float far, bool recalculateProjection = true);

    private:
        float fov, aspect, near, far;
    };
} // namespace VREN
