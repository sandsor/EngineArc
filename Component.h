#pragma once
#include <QMatrix4x4>
struct TransformComponent
{
    QMatrix4x4 Transform;
    TransformComponent() = default;
    TransformComponent(TransformComponent&) = default;
    TransformComponent(const QMatrix4x4& transform)
        :Transform(transform) {}
    operator const QMatrix4x4& () { return Transform; }
};

struct  MeshComponent
{
    float Value;
    MeshComponent() = default;
};