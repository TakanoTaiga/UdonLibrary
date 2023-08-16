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
//    シリアライズ関係
//
//-------------------------------------------------------------------

#pragma once

#include <Udon/Stl/EnableSTL.hpp>
#include <type_traits>
#include <utility>
#include <limits.h>

#include <Udon/Math/Math.hpp>
#include <Udon/Algorithm/CRC.hpp>
#include <Udon/Utility/Parsable.hpp>
#include <Udon/Types/Float.hpp>
#include <Udon/Utility/Concept.hpp>
#include <Udon/Com/Serialization/Traits.hpp>

#if CHAR_BIT != 8
#    error "1byte is must be 8bit"
#endif

namespace Udon
{

    namespace Details
    {

        /// @brief bool型
        UDON_CONCEPT_BOOL
        inline constexpr size_t CapacityImpl(Bool)
        {
            return 1;
        }

        /// @brief 整数型かつbool型でない型
        UDON_CONCEPT_INTEGRAL_NOT_BOOL
        inline constexpr size_t CapacityImpl(IntegralNotBool)
        {
            return sizeof(IntegralNotBool) * CHAR_BIT;
        }

        /// @brief 浮動小数点型
        UDON_CONCEPT_FLOATING_POINT
        inline constexpr size_t CapacityImpl(FloatingPoint)
        {
            return sizeof(Udon::float32_t) * CHAR_BIT;
        }

        /// @brief Capacity 関数から呼び出し可能な型
        template <typename CapacityCallable, typename std::enable_if<Details::CapacityCallable<CapacityCallable>::value, std::nullptr_t>::type* = nullptr>
        inline constexpr size_t CapacityImpl(CapacityCallable&& obj)
        {
            return Capacity(obj);
        }

        /// @tparam Capacitive capacity メソッドを持つ型
        template <typename Capacitive, typename std::enable_if<Details::HasMemberFunctionCapacity<Capacitive>::value, std::nullptr_t>::type* = nullptr>
        inline constexpr size_t CapacityImpl(Capacitive&& obj)
        {
            return obj.capacity();
        }

        /// @brief 組み込み配列型
        UDON_CONCEPT_ARRAY
        inline constexpr size_t CapacityImpl(const Array& obj)
        {
            // 各要素の容量 * 配列の要素数
            return CapacityImpl(*obj) *
                   std::extent<Array>::value;
        }

        /// @brief 可変長引数展開用
        inline constexpr size_t CapacityArgsUnpack()
        {
            return 0;
        }

        /// @brief 可変長引数展開用
        template <typename T>
        inline constexpr size_t CapacityArgsUnpack(T&& obj)
        {
            return Details::CapacityImpl(std::forward<T>(obj));
        }

        /// @brief 可変長引数展開用
        template <typename Head, typename... Args>
        inline constexpr size_t CapacityArgsUnpack(Head&& first, Args&&... args)
        {
            return CapacityArgsUnpack(std::forward<Head>(first)) + CapacityArgsUnpack(std::forward<Args>(args)...);
        }

    }    // namespace Details


    /// @brief シリアライズ後のビット数を求める
    /// @tparam Args シリアライズ対象の型
    /// @param args シリアライズ対象の値
    /// @return シリアライズ後のビット数
    template <typename... Args>
    inline constexpr size_t CapacityBits(Args&&... args)
    {
		return Details::CapacityArgsUnpack(std::forward<Args>(args)...);
	}

    /// @brief チェックサムを含めたシリアライズ後のバイト数を求める
    /// @tparam T シリアライズ対象の型
    /// @return シリアライズ後のバイト数
    template <typename T>
    inline constexpr size_t CapacityWithChecksum()
    {
        //static_assert(std::is_arithmetic<T>::value or Details::HasMemberFunctionCapacity<T>::value or Details::CapacityCallable<T>::value, "Capacity is not defined.");

        return Udon::Ceil(CapacityBits(T{}) / static_cast<double>(CHAR_BIT)) + Udon::CRC8_SIZE;
    }

}    // namespace Udon
