#include <cstdint>

// typedef struct {
//     uint8_t width;
//     uint8_t height;
//     uint16_t offset;
// }FONT_CHAR_INFO;
//
// typedef struct {
//     uint16_t start_character;
//     uint16_t end_character;
//     FONT_CHAR_INFO *descriptor;
// }FONT_CHAR_INFO_LOOKUP;
//
// typedef struct {
//     uint16_t start_character;
//     uint16_t end_character;
//     const FONT_CHAR_INFO_LOOKUP *character_block_lookup;
//     const FONT_CHAR_INFO *descriptor;
//     const uint8_t *bitmaps_array;
// }FONT_INFO;

struct FONT_CHAR_INFO
{
    uint8_t  width;
    uint8_t  height;
    uint16_t offset;
};

struct FONT_INFO
{
    const uint16_t start_character;
    const uint16_t end_character;
    const uint8_t *bitmaps_array;
};

// extern const std::uint8_t consolas_9ptBitmaps[];
// extern const FONT_CHAR_INFO consolas_9ptDescriptors[];
// extern const FONT_INFO consolas_9ptFontInfo;