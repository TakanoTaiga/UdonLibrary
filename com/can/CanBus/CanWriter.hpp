/// @file   CanWriter.hpp
/// @date   2023/01/15
/// @brief  CAN通信送信クラス
/// @author 大河 祐介

#pragma once

#include "CanInfo.hpp"
#include "CanBusInterface.hpp"

template<class MessageTy>
class CanWriter {

		CanBusInterface& bus                      ;
		CanNodeInfo      node                     ;
		uint8_t          buffer[sizeof(MessageTy)];

	public:

		/// @param id 信号識別ID
		CanWriter(CanBusInterface& bus, const uint32_t id)
			: bus   { bus                           }
			, node  { id, buffer, sizeof(MessageTy) }
			, buffer{                               }
		{
			bus.joinTX(node);
		}

		~CanWriter()
		{
			bus.detachTX(node);
		}

		constexpr size_t length() const noexcept
		{
			return sizeof(MessageTy);
		}

		/// @brief 通信できているか
		operator bool() const noexcept
		{
			return micros() - node.timestampUs < 50000;
		}

		/// @brief メッセージ構造体をセット
		void setMessage(const MessageTy& message) noexcept
		{
			memcpy(buffer, &message, length());
		}

		/// @brief 受信内容を表示
		void show(char end = {}) const noexcept
		{
			MessageTy msg;
			memcpy(&msg, buffer, sizeof msg);
			msg.show();
			Serial.print(end);
		}

};
