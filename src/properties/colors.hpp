
// external
#include <raylib.h>

namespace property
{
    class Colors
    {
    public:

        const Color WORKER_COLOR = RED;
        const Color SELECTED_WORKER_COLOR = GetColor(1863348735);

        const Color SELECTION_AREA_COLOR{0, 0, 255, 100};
    };

    static inline Colors color;

} // namespace property