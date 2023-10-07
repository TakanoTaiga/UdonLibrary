#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <vector>
#include <stdint.h>

#include <Udon/Algorithm/ArrayView.hpp>

namespace Udon
{

    struct LoraNode
    {
        uint8_t* data;          // バッファを指すポインタ
        uint8_t  size;          // バッファの長さ
        uint32_t transmitMs;    // 最終通信時刻
    };

    /// @brief Loraのインターフェース
    /// @details
    ///     Loraの通信を行うクラスはこのインターフェースを実装する
    class LLora
    {
    public:
        virtual ~LLora() {}

        /// @brief バスの有効性を取得
        virtual operator bool() const = 0;

        /// @brief 送信ノードを登録
        /// @param node
        virtual void joinTx(LoraNode& node) = 0;

        /// @brief 受信ノードを登録
        virtual void joinRx(LoraNode& node) = 0;
    };
}    // namespace Udon
