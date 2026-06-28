#pragma once

namespace gpio
{
    enum class Direction
    {
        Input,
        Output
    };

    enum class Level
    {
        Low = 0,
        High = 1
    };
}