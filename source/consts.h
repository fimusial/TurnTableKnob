#pragma once

#include "vstgui/vstgui.h"

namespace TTK
{
    static const CColor BackgroundColor = CColor(0x17, 0x17, 0x17);
    static const CColor BorderColor = CColor(0x03, 0x2e, 0x03);
    static const CColor TextColor = CColor(0xff, 0xff, 0xff);
    static const CColor PrimaryColor = CColor(0xdd, 0xdd, 0xdd);
    static const CColor SecondaryColor = CColor(0x0b, 0x71, 0x0b);

    static const char* DEFAULT_FILE_PATH = "select a .wav file...";
    static const double SAMPLE_WAVEFORM_RATIO = 128.0;
    static const int SCROLL_SPEED = 4096;
    static const int ZOOM_SPEED = 4096;
    static const int TIMER_DELAY_MS = 16;

    static const size_t MIN_WINDOW_SIZE = 32768;
    static const size_t MAX_WINDOW_SIZE = 131072;
    static const double ACCELERATION_THRESHOLD = 5.0e-12;
    static const double DE_CLICKER_STEP = 1.0 / 512.0;
}
