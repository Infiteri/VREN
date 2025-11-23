#pragma once
#undef near
#undef far

#include "Base.h"
#include "Camera.h"

namespace VREN
{
    class VREN_API OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera(float width, float height, float near = 0.001, float far = 1000.0f);

        void UpdateProjection();
        void UpdateProjection(float aspect);
        void UpdateProjection(float width, float height, float near, float far);

        void SetAspect(float w, float h, bool recalcualteProjection = true);

        inline float Width() const { return width; }
        inline float Height() const { return height; }
        inline float Near() const { return near; }
        inline float Far() const { return far; }

        void SetWidth(float w, bool recalcualteProjection = true);
        void SetHeight(float h, bool recalcualteProjection = true);
        void SetNear(float n, bool recalcualteProjection = true);
        void SetFar(float f, bool recalcualteProjection = true);

    private:
        float width, height, near, far;
    };
} // namespace VREN
