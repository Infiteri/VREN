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
        projection = Matrix4::Ortho(-width / 2, width / 2, -height / 2, height / 2, near, far);
    }

    void OrthographicCamera::SetAspect(float w, float h, bool recalcualteProjection)
    {
        if (recalcualteProjection)
            UpdateProjection(w, h, near, far);
    }

} // namespace VREN
