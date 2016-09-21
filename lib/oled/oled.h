#include "lib/stream.h"

/**
 * @file
 * @author  Johan Lofstad, Sondre Baugstø and Sondre Russvoll
 * @version 1.0
 *
 * An interface to communicate with the oled display
 */
class OLED: Stream {
public:
    /**
     * A Singleton implementation of this class
     *
     */
    static OLED& GetInstance(){
        static OLED instance;
        return instance;
    }

};
