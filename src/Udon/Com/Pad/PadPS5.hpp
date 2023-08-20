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
#include <Udon/Com/I2c.hpp>
#include <Udon/Com/Can.hpp>
#include <Udon/Com/Message/PadPS5.hpp>

// Q 親クラスが関数を提供するのはおかしい？
// A そうですね。継承するときには、親クラスの関数をオーバーライドすることを想定しています。
// Q コードが冗長になるのを防ぐために、親クラスが複数の関数を提供する用意しているのですが、それはよいのでしょうか？
// A そうですね。コードが冗長になるのを防ぐために、親クラスが複数の関数を提供することは、よいと思います。

namespace Udon
{

    /// @brief I2c経由 PS5コントローラー受信クラス
    /// @details ボタン等の情報を取得する関数は IPadPS5クラス に実装されています。
    class I2cPadPS5
        : public IPadPS5
    {
        I2cMasterReader<Message::PadPS5> reader;

    public:
        /// @brief コンストラクタ
        /// @param bus I2cバス
        /// @param address スレーブアドレス
        I2cPadPS5(II2cBus& bus, uint8_t address)
            : reader{ bus, address }
        {
        }

        /// @brief 更新
        void update()
        {
            reader.update();
            IPadPS5::update(reader.getMessage());
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
            IPadPS5::update(reader.getMessage());
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
