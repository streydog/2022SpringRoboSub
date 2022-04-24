#include "Initializable.h"
#include <vector>

class HardwareRegistry {
    public:
        HardwareRegistry();
        void initializeAll();
        void add(Initializable periphery);
    private:
        vector<Initializable> hardware;
};