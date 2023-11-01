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
//    CAN通信 Teensy用バスクラス 実装部
//
//-------------------------------------------------------------------

namespace Udon
{

    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>::CanBusTeensy()
    {
        self = this;
    }

    /// @brief コピーコンストラクタ
    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>::CanBusTeensy(const CanBusTeensy&)
    {
        self = this;
    }

    /// @brief デストラクタ
    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>::~CanBusTeensy()
    {
        end();
    }

    /// @brief 通信開始
    /// @param baudrate 通信レート
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::begin(const uint32_t baudrate)
    {
        // バス初期化
        bus.begin();
        bus.setBaudRate(baudrate);

        // 受信開始
        if (rxNodes)
        {
            // 受信フィルタ設定 (ノード数が8以下の場合のみ)
            if (rxNodes.size() <= 8)
            {
                bus.setFIFOFilter(REJECT_ALL);
                for (size_t i = 0; i < rxNodes.size(); ++i)
                {
                    bus.setFIFOUserFilter(
                        /* Filter       */ i,
                        /* ID           */ rxNodes[i].node->id,
                        /* Mask         */ 0x7FF,
                        /* Frame type   */ STD,
                        /* Remote frame */ NONE);
                    // 複数のフィルタを設定する関数が用意されているが、適切に受信できていなかったので1つずつ設定している (2023/08/19)
                }
            }
            else
            {
                bus.setFIFOFilter(ACCEPT_ALL);
            }

            // FIFOバッファ有効化
            bus.enableFIFO();
            bus.enableFIFOInterrupt();

            // 受信割り込み設定
            bus.onReceive(
                [](const CAN_message_t& msg)
                {
                    self->rxBuffer.push_back(msg);
                });
        }
    }

    /// @brief 通信終了
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::end()
    {
        bus.disableFIFOInterrupt();
        bus.disableFIFO();
    }

    /// @brief バス更新
    /// @param transmitIntervalMs 送信間隔 [ms]
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::update(uint32_t transmissionIntervalUs)
    {
        onReceive();
        if (txNodes && micros() - transmitUs >= transmissionIntervalUs)
        {
            onTransmit();
            transmitUs = micros();
        }
    }

    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>::operator bool() const
    {
        if (rxNodes)
            return millis() - receiveMs < 100;
        else if (txNodes)
            return micros() - transmitUs < 100000;
        else
            return false;
    }

    /// @brief バス情報を表示する
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::show() const
    {
        Serial.print("CanBusTeensy\n");

        for (auto&& node : txNodes)
        {
            Serial.print("\tTX  ");

            Serial.printf("0x%03x ", static_cast<int>(node->id));

            Serial.printf("%2zu byte ", node->length);

            Serial.print(node->length > SingleFrameSize ? "(multi  frame) " : "(single frame) ");

            Serial.print("[");
            for (size_t i = 0; i < node->length; ++i)
            {
                Serial.printf("%4d", node->data[i]);
            }
            Serial.print(" ]");

            Serial.println();
        }

        for (auto&& rxNode : rxNodes)
        {
            Serial.print("\tRX  ");

            Serial.printf("0x%03x ", static_cast<int>(rxNode.node->id));

            Serial.printf("%2zu byte ", rxNode.node->length);

            Serial.print(rxNode.node->length > SingleFrameSize ? "(multi  frame) " : "(single frame) ");

            Serial.print("[");
            for (size_t i = 0; i < rxNode.node->length; ++i)
            {
                Serial.printf("%4d", rxNode.node->data[i]);
            }
            Serial.print(" ]");

            Serial.println();
        }
    }

    /// @brief 送信ノードをバスに参加させる
    /// @param node 送信ノード
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::joinTx(CanNode& node)
    {
        txNodes.push_back(&node);
    }

    /// @brief 受信ノードをバスに参加させる
    /// @param node 受信ノード
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::joinRx(CanNode& node, void (*onReceive)(void*), void* p)
    {
        rxNodes.push_back({ &node, onReceive, p });
    }

    /// @brief 送信ノードをバスから離脱させる
    /// @remark 送信ノードのインスタンスポインタを基に削除します。
    /// @param node 送信ノード
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::leaveTx(const CanNode& node)
    {
        txNodes.erase(std::find(txNodes.begin(), txNodes.end(), &node));
    }

    /// @brief 受信ノードをバスから離脱させる
    /// @remark 受信ノードのインスタンスポインタを基に削除します。
    /// @param node 受信ノード
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::leaveRx(const CanNode& node)
    {
        rxNodes.erase(std::find_if(rxNodes.begin(), rxNodes.end(), [&node](const RxNodePtr& rxNode)
                                   { return rxNode.node == &node; }));
    }

    /// @brief 受信処理
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::onReceive()
    {
        const auto ms = millis();

        for (auto&& msg : rxBuffer)
        {
            // IDに対応する受信ノードを探す
            auto rxNode = std::find_if(rxNodes.begin(), rxNodes.end(), [&msg](const RxNodePtr& rx)
                                       { return rx.node->id == msg.id; });
            if (rxNode == rxNodes.end())
            {
                continue;
            }

            // 分割されたフレームを結合(マルチフレームの場合)
            Udon::Detail::Unpacketize(
                { msg.buf },
                { rxNode->node->data, rxNode->node->length },
                SingleFrameSize);

            // 登録されている受信クラスのコールバック関数を呼ぶ
            // 最終フレームの到達時にコールバックを呼ぶため、受信中(完全に受信しきっていないとき)にデシリアライズすることを防いでいる。

            if (rxNode->node->length > SingleFrameSize)
            {
                // マルチフレーム
                const auto frameCount = std::ceil(static_cast<double>(rxNode->node->length) / SingleFrameSize - 1 /*index*/);

                if (msg.buf[0] == frameCount)
                {
                    rxNode->callback();
                }
            }
            else
            {
                // シングルフレーム
                rxNode->callback();
            }

            receiveMs = rxNode->node->transmitMs = ms;
        }
        rxBuffer.clear();
    }

    /// @brief 送信処理
    template <CAN_DEV_TABLE Bus>
    void CanBusTeensy<Bus>::onTransmit()
    {
        const auto ms = millis();

        for (auto&& node : txNodes)
        {
            CAN_message_t msg;
            msg.id  = node->id;
            msg.len = SingleFrameSize;

            // 一度に8バイトしか送れないため、分割し送信
            Udon::Detail::Packetize({ node->data, node->length }, { msg.buf }, SingleFrameSize,
                                    [this, &msg](size_t)
                                    {
                                        bus.write(msg);
                                        delayMicroseconds(200);
                                    });

            node->transmitMs = ms;
        }
    }

    template <CAN_DEV_TABLE Bus>
    CanBusTeensy<Bus>* CanBusTeensy<Bus>::self;

};    // namespace Udon
