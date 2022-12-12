/// @file   BasicReader.hpp
/// @date   2022/12/11
/// @brief  受信クラスへ派生することで各種関数を提供するクラス
/// @author 大河 祐介

#pragma once

#if __has_include(<iostream>)
#	include <iostream>
#endif

template<size_t Size>
class BasicReader {

		uint8_t (&buffer)[Size];

	public:

		/// @brief 送信クラスのバッファをセット
		/// @param buffer 送信クラスのバッファ配列に対する参照
		BasicReader(uint8_t (&buffer)[Size])
			: buffer(buffer)
		{}

		/// @brief バッファのサイズ
		static constexpr size_t size = Size;

		/// @brief 所定の番地の1byteを取得
		/// @param index バイト番地
		inline uint8_t getByteData(const size_t index) const {
			return buffer[index];
		}

		/// @brief 所定の番地の1bitを取得
		/// @param byteIndex バイト番地
		/// @param bitIndex  ビット番地
		inline void getBitData(const size_t byteIndex, const size_t bitIndex) {
			return bitRead(getByteData(byteIndex), bitIndex);
		}

		/// @brief 所定の番地にモーターのデータを取得
		/// @param index モーター番地
		/// @param value 値 (-255 ~ 255)
		inline int16_t getMotorData(const uint8_t index) const {
			const uint8_t dirByteIndex = Size - 1 - index / 8;  /// 配列末端バイトから
			const uint8_t dirBitIndex  = index % 8;             /// 先頭ビットから
			const bool dir = bitRead(getByteData(dirByteIndex), dirBitIndex);
			return getByteData(index) * (dir ? 1 : -1);
		}

		/// @brief 構造体として取得
		/// @details 構造体のバイト数と設定バイト数を合わせる(コンパイル時エラーが投げられます)
		template<class Ty>
		inline Ty getDataFrame() const {
			static_assert(sizeof(Ty) == Size, "Different from the number of bytes set.");
			Ty retval;
			memcpy(&retval, buffer, Size);
			return retval;
		}

		/// @brief  バッファのconst参照を取得
		/// @return バッファのconst参照
		inline const uint8_t (&data()) [Size] {
			return buffer;
		}

		/// @brief セット値出力
		/// @param end 最後に出力される文字
		void show(const char end = {}) const {
			for (const auto& buf : buffer)
				Serial.print(buf), Serial.print('\t');
			Serial.print(end);
		}

#if __has_include(<iostream>)
		friend std::ostream& operator<<(std::ostream& ostm, const BasicReader& r){
			for (const auto& buf : r.buffer) {
				ostm << buf;
			}
			return ostm;
		}
#endif

};
