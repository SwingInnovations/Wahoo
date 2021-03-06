#ifndef SWINGTECH1_STFONTMETRICS_H
#define SWINGTECH1_STFONTMETRICS_H

#if __WINDOWS__
extern "C"{
#include <ft2build.h>
#include FT_FREETYPE_H
};
#else
extern "C"{
#include <ft2build.h>
#include FT_FREETYPE_H
}
#endif

#include <string>
#include <iostream>
#include <map>
#include "../Math/Vector.h"

struct STFontMetrics{
    /*!
     * @brief Gets font information based on given string
     * @param text Input text to analyze
     * @param fontSize Intended font size
     * @param fontFile Intended font
     * @param width Reference to width, maximum width will be stored here.
     * @param height Reference to height, maximum height will be stored here.
     */
    static void bounds(const std::string& text, const int fontSize, const std::string fontFile, int* width, int* height){
        struct Character{
            Vector2<int> size;
            Vector2<int> bearing;
            unsigned int advance;
        };

        std::map<char, Character> characters;

        FT_Library ft;
        if(FT_Init_FreeType(&ft)){
            std::cout << "Failed to load freetype!" << std::endl;
            return; }

        stReal x = 0;

        FT_Face face;
        if(FT_New_Face(ft, fontFile.c_str(), 0, &face)){
            std::cout << "Failed to load face! " << std::endl;
            return; }
        FT_Set_Pixel_Sizes(face, 0, 128);

        stReal fs = fontSize/128.0f;

        for(unsigned int i = 0; i < 128; i++){
            if(FT_Load_Char(face, i, FT_LOAD_RENDER)){ std::cout << "Failed to char face!" << std::endl; continue; }
                Character character = {Vector2<int>(face->glyph->bitmap.width,
                                                    face->glyph->bitmap.rows),
                                        Vector2<int>(face->glyph->bitmap_left,
                                                    face->glyph->bitmap_top),
                                       (unsigned int)face->glyph->advance.x};
                characters.insert(std::pair<char, Character>(i, character));
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        int h = *height;
        int w = *width;

        std::string::const_iterator c;
        for(c = text.begin(); c != text.end(); c++){
            Character ch = characters[*c];
            int charHeight = (int)(ch.size.getY() * fs);
            x += ((ch.advance >> 6)) * fs;
            h = (charHeight > h) ? charHeight : h;
        }
        *width = (int)x;
        *height = (int)h;
    }
};

#endif //SWINGTECH1_STFONTMETRICS_H
