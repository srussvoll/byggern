#pragma once
#include <avr/io.h>

namespace Highscore {
    struct Score {
        uint16_t score;
        uint8_t name_length;
        char *name;

        Score(uint16_t score, char *name, uint8_t name_length) : score(score), name_length(name_length), name(name) {};
    };

    class Highscore {
    private:
        Score **score;
        uint8_t length = 0;

        void LoadScore();
    public:
        Highscore();
        void SaveScore(Score &score);
        void StoreScore();
        Score ** GetHighscore(uint8_t first);
        void ClearHighscore();
    };
}