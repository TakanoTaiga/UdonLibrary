#pragma once

#include <udon/traits/HasMember.hpp>

namespace udon
{

	struct MemberViewer
	{

		const char* gap;

		/// @brief
		/// @tparam T 整数型
		/// @param rhs
		/// @return
		template<typename T>
		auto operator()(const T& rhs) -> typename std::enable_if<std::is_scalar<T>::value>::type
		{
            Serial.print(rhs);
            Serial.print(gap);
        }

		/// @brief
		/// @tparam T 配列
		/// @param rhs
		/// @return
		template<typename T>
		auto operator()(const T& rhs) -> typename std::enable_if<std::is_array<T>::value>::type
		{
			for (auto&& it : rhs)
			{
				operator()(it);
			}
		}

		/// @brief
		/// @tparam T メンバにaccessorを持つ型
		/// @param rhs
		/// @return
		template<typename T>
		auto operator()(const T& rhs) -> typename std::enable_if<has_member_iterator_accessor<MemberViewer, T>::value>::type
		{
			// T::accessor が const なメンバ関数でない場合に const rhs から呼び出せないため、const_cast によって const を除去
			const_cast<T&>(rhs).accessor(*this);
		}

		/// @brief 可変長テンプレート引数再帰展開
		/// @tparam Head
		/// @tparam ...Tails
		/// @param head
		/// @param ...tails
		template<typename Head, typename... Tails>
		void operator()(const Head& head, const Tails&... tails)
		{
			operator()(head);
			operator()(tails...);
		}

	};

	template<typename T>
	inline void Show(const T& rhs, const char* gap = "/t")
	{
		MemberViewer viewer{ gap };
		viewer(rhs);
	}

}