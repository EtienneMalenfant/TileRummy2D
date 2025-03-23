#pragma once

namespace gui {

    class IUpdateable {
    public:
        virtual void update() = 0;

        virtual ~IUpdateable() = default;
    };
}