#include "GameObject.h"

Vector2 GameObject::Position()
{
    return position;
}

void GameObject::Position(const Vector2& vec)
{
    position = vec;
}
