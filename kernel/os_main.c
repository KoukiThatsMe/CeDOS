#include "os_text.h"

int main(void) {
    const char* string[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    clear();
    for (int i = 0; i < 50; i++) {
        write("LINE ");
        write(string[i / 10]);
        write(string[i % 10]);
        write("\n");
    }

    write(  "SOMEBODY ONCE TOLD ME "
            "THE WORLD IS GONNA ROLL ME "
            "I AINT THE SHARPEST TOOL IN THE SHED "
            "SHE WAS LOOKING KINDA DUMB "
            "WITH HER FINGER AND HER THUMB "
            "IN THE SHAPE OF AN L ON HER FOREHEAD"
        );
    //backspace();

    return 0;
}