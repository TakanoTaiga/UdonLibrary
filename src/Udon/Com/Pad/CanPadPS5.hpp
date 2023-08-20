//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2022-2023 Okawa Yusuke
//    Copyright (c) 2022-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    PS5コントローラー受信クラス
//
//    Controller --> Sender --> Main
//                              ^^^^
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Com/Pad/IPadPS5.hpp>
#include <Udon/Com/Can/CanReader.hpp>

namespace Udon
{
    /// @brief CAN経由 PS5コントローラー受信クラス
    /// @details ボタン等の情報を取得する関数は IPadPS5クラス に実装されています。
    class CanPadPS5
        : public IPadPS5
    {
        CanReader<Message::PadPS5> reader;

    public:
        CanPadPS5(ICanBus& bus, uint32_t id)
            : reader{ bus, id }
        {
        }

        void update()
        {
            IPadPS5::updateFromMessageOpt(reader.getMessage());
        }

        void show()
        {
            reader.show();
        }
        void showRaw()
        {
            reader.showRaw();
        }
    };

}    // namespace Udon
