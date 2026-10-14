#pragma once

namespace efp {
    struct Format {
        int exponentBits;
        int fractionBits;
        int bias;
    };

    // 4 different formats
    inline constexpr Format binary32{8, 23, 127};
    inline constexpr Format binary64{11, 52, 1023};
    inline constexpr Format extended128{8, 119, 127};
    inline constexpr Format extended1024{11, 1012, 1023};
};
