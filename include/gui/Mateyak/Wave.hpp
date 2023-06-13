//
// Created by Talleux on 13/06/2023.
//

#pragma once

#include <string>
#include <memory>
#include <raylib.h>

namespace Mateyak
{
    class AudioWave {
        public:
            explicit AudioWave(const std::string& path);
            ~AudioWave();

            [[nodiscard]] Wave getWave() const;

        private:
            Wave _wave{};
    };
}
