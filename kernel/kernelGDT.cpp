#include "kernelGDT.hpp"
#include <stdint.h>


extern "C" void GDT_Load();

namespace GDT {
    void Init()
    {
        GDT_Load();
    }
}