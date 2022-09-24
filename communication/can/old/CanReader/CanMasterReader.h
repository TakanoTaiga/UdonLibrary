/**
   @file CanMasterReader.h
   @note マスターIDは常に255
*/

#pragma once

#include <FlexCAN_T4.h>
#include <IntervalTimer.h>

//#ifdef teensy~~?????
#include <vector>
//#else
// #error This board is not supported

class CanReader {
    // インスタンス間通信用メンバ
    static uint8_t MASTER_ID;
    static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256> can;
    static IntervalTimer timer;
    static std::vector<CanReader*> instancePtr;  // インスタンスごとのthisポインタ用配列

    // 通常メンバ
    const uint8_t observID;  // 観察する送信元ID
    const uint8_t readSize;  // 読み取りサイズ
    uint8_t *buf;            // 読み取りバッファ

  public:
    CanReader(uint8_t observID, uint8_t readSize)
      : observID(observID)
      , readSize(readSize)
      , buf(new uint8_t[readSize])
    {
      memset(buf, 0, readSize);
      if (instancePtr.empty()) {  // 最初のインスタンス生成の場合CAN通信を開始
        can.begin();
        can.onReceive(onAllReceive);
        can.setClock(CLK_60MHz);
        can.setBaudRate(500000);
        can.enableFIFO();
        can.enableFIFOInterrupt();
        timer.begin([] {can.events();}, 100); // タイマー割り込みで通信を監視(100us間隔)
      }
      instancePtr.push_back(this);  // インスタンスのアドレスを追加
    }

    const uint8_t& operator[](uint8_t index) const {
      return buf[index];
    }

    void show(const String& str = "") const {
      for (uint8_t i = 0; i < readSize; i++)
        Serial.print(buf[i]), Serial.print('\t');
      Serial.print(str);
    }

  private:
    /**
       @brief 受信割り込み
       @param msg CAN_message_t構造体
    */
    void onReceive(const CAN_message_t& msg) {
      // communication format [8byte]
      // [target address][packet index][data][data][data][data][data][data]
      const uint8_t packetIndex = msg.buf[1];  // 分割したデータの要素番号
      for (uint8_t i = 0; i < 6; i++) {
        const uint8_t bufferIndex = i + packetIndex * 6;
        if (bufferIndex < readSize)  // 配列範囲内参照時
          buf[bufferIndex] = msg.buf[i + 2/*先頭2byteを飛ばす*/];
        else
          break;
      }

    }

    /**
       @brief 受信割り込み(インスタンス間通信用)
       @param msg CAN_message_t構造体
    */
    static void onAllReceive(const CAN_message_t& msg) {
      const uint8_t writerID = msg.id;      // 送信元ID
      const uint8_t targetID = msg.buf[0];  // ターゲットID

      if (targetID == MASTER_ID)            // 自身(マスター)に向けた送信の場合
        for (const auto p : instancePtr)
          if (p->observID == writerID)      // ターゲットIDと観察するIDが一致する場合,そのインスタンスのonReceiveを呼ぶ
            p->onReceive(msg);

    }

};

// static メンバ初期化
uint8_t CanReader::MASTER_ID = 255;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256> CanReader::can;
IntervalTimer CanReader::timer;
std::vector<CanReader*> CanReader::instancePtr;
