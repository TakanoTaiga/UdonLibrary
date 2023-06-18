#pragma once

#include <udon/com/i2c/I2cBus.hpp>
#include <udon/com/serialization/Serializer.hpp>
#include <udon/utility/Show.hpp>


namespace udon
{

    template <class Message>
    class I2cMasterReader
    {

        static constexpr size_t Size = udon::CapacityWithChecksum<Message>();

        udon::II2cBus& bus;

        uint8_t address;

        uint8_t buffer[Size];

    public:

        /// @brief コンストラクタ
        /// @param bus I2cバス
        /// @param address スレーブアドレス
        I2cMasterReader(udon::II2cBus& bus, uint8_t address)
            : bus(bus)
            , address(address)
            , buffer()
        {
        }

        /// @brief 更新
        void update()
        {
            bus.requestFrom(address, Size);
            while (bus.available())
            {
                for (auto&& it : buffer)
                {
                    it = bus.read();
                }
            }
        }

        /// @brief 受信したメッセージを取得
        /// @return 受信したメッセージ
        udon::optional<Message> getMessage() const
        {
            if (bus)
            {
                return udon::Unpack<Message>(buffer);
            }
            else
            {
                return udon::nullopt;
            }
        }

        /// @brief 受信バッファの参照を取得
        /// @return 受信バッファの参照
        const uint8_t (&data() const)[Size]
        {
            return buffer;
        }

        /// @brief 受信内容を表示
        /// @param gap 区切り文字 (default: "\t")
        void show(const char* gap = "\t") const
        {
            if (const auto message = getMessage())
            {
                udon::Show(*message);
            }
            else
            {
                Serial.print(F("receive failed!"));
            }
        }

        /// @brief 受信バッファを表示
        /// @param gap 区切り文字 (default: " ")
        void showRaw(const char* gap = " ") const
        {
            for (auto&& it : buffer)
            {
                Serial.print(buffer);
                Serial.print(gap);
            }
        }
    };

}    // namespace udon
