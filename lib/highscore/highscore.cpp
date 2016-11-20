#ifdef __AVR_ATmega162__

#include "highscore.h"

#include <stdlib.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>
#include <stdio.h>

#include "lib/uart/uart.h"
#include "lib/oled/oled.h"
#include "lib/oled_memory/oled_memory.h"
#include "lib/utilities/new.h"

#define MAX_LENGTH 40
#define MAX_NAME_LENGTH 10

#define WE  EEWE
#define RE  EERE
#define CR  EECR
#define AR  EEAR
#define DR  EEDR
#define MWE EEMWE

void EEPROM_write(uint16_t address, uint8_t data) {
    while(CR & (1 << WE));
    cli();
    AR = address;
    DR = data;
    CR = (CR | (1 << MWE)) & ~(1 << WE);
    CR |= (1 << WE);
    sei();
}

uint8_t EEPROM_read(uint16_t address) {
    while(CR & (1 << WE));
    cli();
    AR = address;
    CR |= (1 << RE);
    sei();
    return DR;
}

namespace Highscore {
    void Score::operator=(const Score &score) {
        this->score = score.score;
        this->name_length = score.name_length;
        memcpy(this->name, score.name, score.name_length);
    }

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
        // Make sure the name isn't longer than allowed.
        if (score.name_length > MAX_NAME_LENGTH) score.name_length = MAX_NAME_LENGTH;

        bool insert_score = true;
        // If length == 0, add anyway.
        if (this->length > 0) {
            // Check if name is already there, and then if new score is better.
            for (int i = 0; i < this->length; ++i) {
                // If name length is different, it is not equal anyway.
                if (this->score[i]->name_length != score.name_length) continue;
                // Compare names if length is the same.
                bool equal = true;
                for (int j = 0; j < score.name_length; ++j) {
                    if (this->score[i]->name[j] != score.name[j]) {
                        equal = false;
                        break;
                    }
                }
                if (!equal) continue;

                // The names are equal, check if new score is better.
                if (score.score > this->score[i]->score) {
                    // Remove this score from the list.
                    // Since all scores are allocated on the heap
                    // we cannot really forget them. So instead
                    // we move this element past the end of the
                    // array.
                    this->length -= 1;
                    for (int j = i; j < this->length; ++j) {
                        Score *temp = this->score[j];
                        this->score[j] = this->score[j + 1];
                        this->score[j + 1] = temp;
                    }
                } else {
                    insert_score = false;
                }

                break;
            }
        }

        if (insert_score) {
            for (int8_t i = this->length; i >= 0; --i) {
                if (i > 0 && score.score > this->score[i - 1]->score) {
                    // Swap scores as explained above:
                    Score *temp = this->score[i];
                    this->score[i] = this->score[i - 1];
                    this->score[i - 1] = temp;
                } else {
                    *(this->score[i]) = score;
                    break;
                }
            }

            if (this->length < MAX_LENGTH) this->length++;
        }
    }

    uint8_t Highscore::GetHighscores(uint8_t first, Score ** &scores) {
        // TODO: Make scores const so it cannot be changed outside of the class.
        scores = this->score;
        return this->length - first;
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