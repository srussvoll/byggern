#pragma once
#include <avr/io.h>

/**
 * \brief Keeps track of the highscore and stores it to the EEPROM using Score objects.
 *
 * The Highscore namespace contains a Highscore class for
 * keeping track of the highscores, and saving them to the
 * EEPROM for persistent storage. It also contains a Score
 * struct for representing a score.
 */
namespace Highscore {
    /**
     * \brief Represents a score.
     * @param score This is the score, 16 bits wide.
     * @param name_length This is the lenght of the name.
     * @param name This is the name of the person who scored score points.
     */
    struct Score {
        uint16_t score;
        uint8_t name_length;
        char *name;

        Score(uint16_t score, char *name, uint8_t name_length) : score(score), name_length(name_length), name(name) {};
    };


    /**
     * \brief Keeps track of the highscore and stores it to the EEPROM.
     * 
     * The Highscore class keeps track of the highscores,
     * and provides a method for saving them to the EEPROM
     * for persistent storage.
     */
    class Highscore {
    private:
        /**
         * Contains a sorted list of scores.
         */
        Score **score;

        /**
         * Length of the score variable.
         */
        uint8_t length = 0;

    public:
        /**
         * Automatically loads highscores from EEPROM.
         */
        Highscore();

        /**
         * Saves the score to the highscore list.
         * @param score The score to save.
         */
        void SaveScore(Score &score);

        /**
         * Store the highscore list to the EEPROM.
         * This can take a few seconds.
         */
        void StoreScores();

        /**
         * Loads the highscore list from the EEPROM.
         */
        void LoadScores();

        /**
         * Returns an array of highscores
         * @param first The first highscore to return by index.
         * @param scores The array to change to point to the highscore list.
         * @return The length of the highscore list.
         */
        uint8_t GetHighscores(uint8_t first, Score ** &scores);

        /**
         * Clears the list of highscores.
         * Of course, this does not clear the stored highscore list
         * from the EEPROM. If you want to clear the EEPROM, please
         * run ClearHighscores() followed by StoreScores().
         */
        void ClearHighscores();
    };
}