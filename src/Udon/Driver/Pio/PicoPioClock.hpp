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
//    raspberry pi pico PIO clock
//
//-------------------------------------------------------------------

#pragma once

#if defined(ARDUINO_ARCH_RP2040)

#    include "SquareWave.pio.hpp"
#    include "StateMachineAllocator.hpp"

namespace Udon
{
    inline bool PicoPioClockBegin(uint8_t pin, uint32_t clockSpeed)
    {
        auto sm = Pio::AllocateStateMachine(Udon::Pio::Sqwave::squarewave_program);
        if (not sm)
        {
            return false;
        }

        auto c = Udon::Pio::Sqwave::squarewave_program_get_default_config(sm->offset);

        sm->pio->sm[sm->index].clkdiv = (uint32_t)(F_CPU * 0.25f * (1 << 16) / clockSpeed);    // 周波数設定(周波数低めのほうが誤差が少ない)

        sm->pio->sm[sm->index].pinctrl = (1 << PIO_SM0_PINCTRL_SET_COUNT_LSB) | (pin << PIO_SM0_PINCTRL_SET_BASE_LSB);    // PIOがIOピンにアクセスできるようにする

        gpio_set_function(pin, sm->pio == pio0 ? GPIO_FUNC_PIO0 : GPIO_FUNC_PIO1);

        // pio_sm_init(pio, sm, offset, &c);
        pio_sm_set_enabled(sm->pio, sm->index, true);

        return true;
    }

}    // namespace Udon

#endif