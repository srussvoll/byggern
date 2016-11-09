#ifdef __AVR_ATmega162__
#include "highscore.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lib/uart/uart.h>
#include "lib/utilities/new.h"

#define MAX_LENGTH 40
#define MAX_NAME_LENGTH 10

#define WE  EEWE
#define RE  EERE
#define CR  EECR
#define AR  EEAR
#define DR  EEDR
#define MWE EEMWE

inline void EEPROM_write(uint16_t address, uint8_t data) {
    while(CR & (1 << WE));
    cli();
    AR = address;
    DR = data;
    CR = (CR | (1 << MWE)) & ~(1 << WE);
    CR |= (1 << WE);
    sei();
}

inline uint8_t EEPROM_read(uint16_t address) {
    while(CR & (1 << WE));
    cli();
    AR = address;
    CR |= (1 << RE);
    sei();
    return DR;
}

namespace Highscore {
    Highscore::Highscore() {
        /*
         * Score:
         * 2 byte score
         * 1 byte name length
         * x byte name
         */

        this->score = (Score **) malloc((MAX_LENGTH + 1) * sizeof(Score*));
        for (int i = 0; i < MAX_LENGTH + 1; ++i) {
            char *name = (char *) malloc(MAX_NAME_LENGTH * sizeof(char));
            this->score[i] = new Score(0, name, MAX_NAME_LENGTH);
        }

        this->LoadScores();
    }

    void Highscore::SaveScore(Score &score) {
        if (score.name_length > MAX_NAME_LENGTH) score.name_length = MAX_NAME_LENGTH;

        // If length == 0, add anyway.
        if (this->length == 0) {
            memcpy(this->score[0], &score, sizeof(Score));
            this->length += 1;
        } else {
            // Check if name is already there, and then if new score is better.
            bool insert_score = true;
            for (int i = 0; i < this->length; ++i) {
                // If name length is different, not equal.
                if (this->score[i]->name_length != length) continue;

                // Compare names if length is the same.
                bool equal = true;
                for (int j = 0; j < length; ++j) {
                    if (this->score[i]->name[j] != score.name[j]) {
                        equal = false;
                        break;
                    }
                }
                if (!equal) continue;

                // The names are equal, check if new score is better.
                if (score.score > this->score[i]->score) {
                    // Remove this score from the list.
                    for (int j = i; j < this->length - 1; ++j) {
                        this->score[j] = this->score[j + 1];
                    }

                    break;
                } else {
                    insert_score = false;
                }
            }

            if (insert_score) {
                for (int8_t i = this->length; i >= 0; --i) {
                    if (i > 0 && score.score > this->score[i - 1]->score) {
                        // Swap scores:
                        Score *temp = this->score[i];
                        this->score[i] = this->score[i - 1];
                        this->score[i - 1] = temp;
                    } else {
                        memcpy(this->score[i], &score, sizeof(Score));
                        break;
                    }
                }

                if (this->length < MAX_LENGTH) this->length++;
            }
        }
    }

    uint8_t Highscore::GetHighscores(uint8_t first, Score ** &scores) {
        scores = this->score;
        return this->length;
    }

    void Highscore::StoreScores() {
        uint16_t index = 0;
        EEPROM_write(index, this->length);
        index += 1;

        for (int i = 0; i < this->length; ++i) {
            EEPROM_write(index, (uint8_t)(this->score[i]->score >> 8));
            index += 1;
            EEPROM_write(index, (uint8_t)(this->score[i]->score & 0xFF));
            index += 1;

            EEPROM_write(index, this->score[i]->name_length);
            index += 1;

            for (int j = 0; j < this->score[i]->name_length; ++j) {
                EEPROM_write(index, (uint8_t) this->score[i]->name[j]);
                index += 1;
            }
        }
    }

    void Highscore::LoadScores() {
        uint16_t index = 0;
        this->length = EEPROM_read(index);
        index += 1;

        for (uint8_t i = 0; i < this->length; ++i) {
            this->score[i]->score = (EEPROM_read(index) << 8);
            index += 1;
            this->score[i]->score |= EEPROM_read(index);
            index += 1;

            this->score[i]->name_length = EEPROM_read(index);
            index += 1;

            for (uint8_t j = 0; j < this->score[i]->name_length; ++j) {
                this->score[i]->name[j] = (char) EEPROM_read(index);
                index += 1;
            }
        }
    }
}

void Highscore::Highscore::ClearHighscores() {
    this->length = 0;
}

#endif