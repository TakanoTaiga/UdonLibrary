#pragma once

#include <stdint.h>
#include <udon/utility/Parsable.hpp>

namespace udon
{

    namespace message
    {

        struct Solenoid
        {

            /// @brief ソレノイドの位置
            bool release;

#ifdef ARDUINO
            /// @brief デバッグ出力
            void show() const
            {
                Serial.print(F("Sole: ")), Serial.print(release), Serial.print('\t');
            }
#endif

            UDON_PARSABLE(release);
        };

    }    // namespace message

}    // namespace udon
