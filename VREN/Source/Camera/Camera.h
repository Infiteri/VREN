#pragma once

#include "Base.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"

namespace VREN
{
    class VREN_API Camera
    {
    public:
        Camera() {};
        virtual ~Camera() {};

        void SetPosition(const Vector3 &pos);
        void SetRotation(const Vector3 &pos);

        void UpdateView();
        virtual void UpdateProjection() {};
        virtual void SetAspect(float width, float height, bool updateProjection = true) {};

        inline void SetProjection(const Matrix4 &m) { projection = m; };

        inline const Matrix4 &GetProjection() const { return projection; };
        inline const Matrix4 &GetView() const { return view; };

        inline Vector3 &GetPosition() { return position; };
        inline Vector3 &GetRotation() { return rotation; };

    protected:
        Matrix4 projection, view;
        Vector3 position, rotation;
    };
} // namespace Core