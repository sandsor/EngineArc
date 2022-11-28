#pragma once
#include <QMatrix4x4>
#include <entt/core/type_traits.hpp>

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

struct TagComponent
{
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag)
	    :Tag(tag){}

};
