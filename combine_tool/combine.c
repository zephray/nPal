#include <stdio.h>
#include <stdlib.h>

// `which ls` *.tns -l | awk '{print "\""$9"\","}'

const char *files[] = {
    "ABC.MKF.tns",
    "BALL.MKF.tns",
    "DATA.MKF.tns",
    "desc.dat.tns",
    "F.MKF.tns",
    "FBP.MKF.tns",
    "FIRE.MKF.tns",
    "GOP.MKF.tns",
    "M.MSG.tns",
    "MAP.MKF.tns",
    "MGO.MKF.tns",
    "PAT.MKF.tns",
    "RGM.MKF.tns",
    "RNG.MKF.tns",
    "SSS.MKF.tns",
    "WOR16.ASC.tns",
    "WOR16.FON.tns",
    "WORD.DAT.tns",
    NULL
};

int main() {
    int i;
    FILE *dest = fopen("resources.bin.tns", "wb+");
    size_t offist = 0;
    for (i = 0; files[i]; ++i) {
        FILE *current_src = fopen(files[i], "rb");

        fseek(current_src, 0, SEEK_END);
        size_t current_size = ftell(current_src);
        fseek(current_src, 0, SEEK_SET);

        void *data = malloc(current_size);
        fread(data, current_size, 1, current_src);

        fwrite(data, current_size, 1, dest);
        free(data);
        offist += current_size;
        fclose(current_src);
    }
    return 0;
}
