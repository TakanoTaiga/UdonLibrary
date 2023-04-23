#pragma once

#define UDON_INCLUDED

/////////////////////////////////////////////////////////////////////
//
//  アクチュエーター / actuator
//
/////////////////////////////////////////////////////////////////////

/// @brief モーター
#include <udon\actuator\Motor.hpp>


/////////////////////////////////////////////////////////////////////
//
//  アルゴリズム / algorithm
//
/////////////////////////////////////////////////////////////////////

/// @brief フィードバック制御器
#include <udon\algorithm\PidController.hpp>

/// @brief 移動平均
#include <udon\algorithm\MovingAverage.hpp>


/////////////////////////////////////////////////////////////////////
//
//  通信 / com
//
/////////////////////////////////////////////////////////////////////

/// @brief CAN通信
#include <udon\com\can\Can.hpp>

/// @brief UART通信
#include <udon\com\uart\Uart.hpp>

/// @brief シリアライザー
#include <udon\com\serializer\Serializer.hpp>

/// @brief コントローラー
#include <udon\com\pad\PadPS5.hpp>


/////////////////////////////////////////////////////////////////////
//
//  ファイルシステム / filesystem
//
/////////////////////////////////////////////////////////////////////

/// @brief SDカード
#include<udon/filesystem/sd/parameter.hpp>


/////////////////////////////////////////////////////////////////////
//
//  数学 / math
//
/////////////////////////////////////////////////////////////////////

/// @brief 算術関数
#include <udon\math\Math.hpp>


/////////////////////////////////////////////////////////////////////
//
//  センサー / sensor
//
/////////////////////////////////////////////////////////////////////

/// @brief 算術関数
#include <udon\sensor\imu\Imu.hpp>


/////////////////////////////////////////////////////////////////////
//
//  ユーザー定義型 / types
//
/////////////////////////////////////////////////////////////////////

#include <udon\types\types.hpp>


/////////////////////////////////////////////////////////////////////
//
//  システム / system
//
/////////////////////////////////////////////////////////////////////

/// @brief ループ周期制御
#include <udon\algorithm\LoopCycleController.hpp>


/////////////////////////////////////////////////////////////////////
//
//  トレイト / trait
//
/////////////////////////////////////////////////////////////////////

/// @brief メンバー関数の有無
#include <udon\traits\HasMember.hpp>


/////////////////////////////////////////////////////////////////////
//
//  ユーティリティ / utility
//
/////////////////////////////////////////////////////////////////////

/// @brief デバッグ
#include <udon\utility\Show.hpp>
