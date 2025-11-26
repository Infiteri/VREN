#include "OrthographicCamera.h"

namespace VREN
{
    OrthographicCamera::OrthographicCamera(float width, float height, float near, float far)
    {
        UpdateProjection(width, height, near, far);
    }

    void OrthographicCamera::UpdateProjection(float width, float height, float near, float far)
    {
        this->width = width;
        this->height = height;
        this->near = near;
        this->far = far;

        UpdateProjection();
    }

    void OrthographicCamera::UpdateProjection(float aspect) { UpdateProjection(); }

    void OrthographicCamera::UpdateProjection()
    {

        float left = -width / 2.0f / zoom;
        float right = width / 2.0f / zoom;
        float bottom = -height / 2.0f / zoom;
        float top = height / 2.0f / zoom;

        projection = Matrix4::Ortho(left, right, bottom, top, near, far);
    }

    void OrthographicCamera::SetAspect(float w, float h, bool recalcualteProjection)
    {
        if (recalcualteProjection)
            UpdateProjection(w, h, near, far);
    }

    void OrthographicCamera::SetWidth(float w, bool recalcualteProjection)
    {
        this->width = w;
        if (recalcualteProjection)
            UpdateProjection();
    }

    void OrthographicCamera::SetHeight(float h, bool recalcualteProjection)
    {
        this->height = h;
        if (recalcualteProjection)
            UpdateProjection();
    }

    void OrthographicCamera::SetNear(float n, bool recalcualteProjection)
    {
        this->near = n;
        if (recalcualteProjection)
            UpdateProjection();
    }

    void OrthographicCamera::SetFar(float f, bool recalcualteProjection)
    {
        this->far = f;
        if (recalcualteProjection)
            UpdateProjection();
    }

    void OrthographicCamera::SetZoom(float z, bool recalcualteProjection)
    {
        this->zoom = z;
        if (recalcualteProjection)
            UpdateProjection();
    }

} // namespace VREN
