/// @file   Gyro.h
/// @date   2022/09/22
/// @brief  ジャイロセンサから旋回角を取得するクラス
/// @author 大河 祐介

#pragma once

#define SERIAL_INSTANCE Serial1

template<class HardwareSerial>
class _Gyro {
		static HardwareSerial& serial; /// シリアルインスタンス
		static double rawYaw;          /// 旋回角
		static double initOffset;      /// 起動時の旋回角オフセット
		double offset;                 /// インスタンスごとの旋回角オフセット
	public:
		_Gyro() noexcept
			: offset()
		{}

		/// @brief 初期化
		/// @note  必須
		static void begin() {
			serial.begin(115200);
			update();
			initOffset = rawYaw;
		}

		/// @brief 旋回角を更新する
		/// @note  必須
		static void update() {
			while (serial.available())
			{
				if (serial.read() == 'H')
				{
					const auto receiveValue = (serial.read() << 8) | (serial.read() << 0);
					rawYaw = static_cast<int16_t>(receiveValue) / 100.0 - initOffset;  /// 復号化
				}
			}
		}

		/// @brief 旋回角を0にする
		void clear() {
			offset = rawYaw;
		}

		/// 旋回角取得
		/// @return 旋回角 [-180~180deg]
		double yaw() const {
			const double yaw = rawYaw - offset;
			return fmod(fmod(yaw + 180, 360) - 360, 360) + 180;
		}
};

template<class HardwareSerial> HardwareSerial& _Gyro<HardwareSerial>::serial     = SERIAL_INSTANCE;
template<class HardwareSerial> double          _Gyro<HardwareSerial>::rawYaw     = 0;
template<class HardwareSerial> double          _Gyro<HardwareSerial>::initOffset = 0;

using Gyro = _Gyro<decltype(SERIAL_INSTANCE)>;

#undef SERIAL_INSTANCE
