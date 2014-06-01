#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NUM_REGIONS 19
#define TRUE 1
#define REGIONS_TOPLEFT_X {0,0,0,1,1,1,1,2,2,2,2,2,3,3,3,3,4,4,4}
#define REGIONS_TOPLEFT_Y {8,6,4,9,7,5,3,10,8,6,4,2,9,7,5,3,8,6,4}

typedef struct _vertex {
    int x, y;
} vertex;

static void verticesOfRegion(int regionID, vertex vertexCoords[6]);

int main(int argc, char *argv[])
{
    int regionID = 0;
    int i;
    vertex coords[6];

    while (regionID < NUM_REGIONS) {
        printf("Region %d:\n", regionID);
        verticesOfRegion(regionID, coords);
        i = 0;
        while (i < 6) {
            printf("\tvertexCoords[%d]: {%d, %d}\n", i,
                   coords[i].x, coords[i].y);
            i++;
        }
        regionID++;
    }
    return EXIT_SUCCESS;
}

// Given a region's ID, sets vertexCoords to the vertex coords
// that belong to the region
static void verticesOfRegion(int regionID, vertex vertexCoords[6]) {
    // Coords of top right of each region
    int regionsX[NUM_REGIONS] = REGIONS_TOPLEFT_X;
    int regionsY[NUM_REGIONS] = REGIONS_TOPLEFT_Y;
    int i = 0;
    while (i < 3) {
        vertexCoords[i].x = regionsX[regionID];
        vertexCoords[i + 3].x = regionsX[regionID] + 1;
        vertexCoords[i].y = regionsY[regionID] - i;
        vertexCoords[i + 3].y = regionsY[regionID] - i;
        i++;
    }
}