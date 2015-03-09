#ifndef LIBPOINTSCAN_CORE_H
#define LIBPOINTSCAN_CORE_H

#ifdef __cplusplus

#include <iostream>
#include <string>

#define SC_HDR_SIZE sizeof(ps::SC_HDR)
#define SC_AMP_SIZE sizeof(ps::SC_AMP)
#define SC_CAR_SIZE sizeof(ps::SC_CAR)
#define SC_RGB_SIZE sizeof(ps::SC_RGB)
#define SC_NOR_SIZE sizeof(ps::SC_NOR)
#define SC_POL_SIZE sizeof(ps::SC_POL)
#define SC_TEX_SIZE sizeof(ps::SC_TEX)
#define SC_VAL_SIZE sizeof(ps::SC_VAL)

namespace ps {

enum {
       NON = 0xF000,
       VAL = 0x0001,
       POL = 0x0002,
       AMP = 0x0004,
       RGB = 0x0008,
       CAR = 0x0010,
       NOR = 0x0020,
       TEX = 0x0040,
       INT = 0x0080, //Interpolated value
       COR = 0x0100, //Corrected value
       ALL = 0x01FF
     };

// Define the file header structure for scan file
struct sc_header
{
    int hgt;                   // Height
    int wdt;                   // Width
    unsigned int cur;          // Last used position
    unsigned int ord[10];      // Successive element's types in file
    float sop[16];             // Sensor's Orientation and Position matrix

    sc_header()
    {
        hgt = 0;
        wdt = 0;
        cur = 0;
        ord[0] = ord[1] = ord[2]  = ord[3]  = ord[4]  = ord[5]  = 0;
        sop[1] = sop[2] = sop[3]  = sop[4]  = sop[6]  = sop[7]  = 0.0f;
        sop[8] = sop[9] = sop[11] = sop[12] = sop[13] = sop[14] = 0.0f;
        sop[0] = sop[5] = sop[10] = sop[15] = 1.0f;
    }
};

// Define the elements structure for scan file
struct sc_amplitude
{
    float amp;          // Laser beam intensity [0-1]

    sc_amplitude() { amp = 0.f; }
};
struct sc_cartesian
{
    float car[3];       // Cartesian coordinates: X[m], Y[m], Z[m]

    sc_cartesian() { car[0] = car[1] = car[2] = 0.f; }
};
struct sc_color
{
    unsigned char rgb[3];       // Color: R[0-255], G[0-255], B[0-255]

    sc_color() { rgb[0] = rgb[1] = rgb[2] = 0; }
};
struct sc_normal
{
    float nor[3];      // Normal in cartesian coordinates: X[m], Y[m], Z[m]

    sc_normal() { nor[0] = nor[1] = nor[2] = 0.f; }
};
struct sc_polar
{
    float pol[3];       // Polar coordinates: Range[m], Phi[rad], Theta[rad]
                        // Phi: [0, 2PI] ; Theta: [0, PI]

    sc_polar() { pol[0] = pol[1] = pol[2] = 0.f; }
};
struct sc_texture
{
    unsigned int tex[3];// Texture coordinates: U[pxl], V[pxl], Image[idx]

    sc_texture() { tex[0] = tex[1] = tex[2] = 0; }
};
struct sc_validity
{
    unsigned int val;   // Point validity [0|1]

    sc_validity() { val = 0; }
};

// Define the types and their sizes
typedef struct sc_header    SC_HDR;
typedef struct sc_amplitude SC_AMP;
typedef struct sc_cartesian SC_CAR;
typedef struct sc_color     SC_RGB;
typedef struct sc_normal    SC_NOR;
typedef struct sc_polar     SC_POL;
typedef struct sc_texture   SC_TEX;
typedef struct sc_validity  SC_VAL;

// Define the scan structure
struct sc_scan
{
    SC_HDR  hdr;               // Header
    SC_AMP *amp;               // Intensity data pointer
    SC_CAR *car;               // Cartesian coordinate data pointer
    SC_RGB *rgb;               // Color data pointer
    SC_NOR *nor;               // Normal data pointer
    SC_POL *pol;               // Polar coordinate data pointer
    SC_TEX *tex;               // Texture data pointer
    SC_VAL *val;               // Validity data pointer

    sc_scan()
    {
        amp = 0;
        car = 0;
        rgb = 0;
        nor = 0;
        pol = 0;
        val = 0;
        tex = 0;
    }
    ~sc_scan()
    {
        free(amp);
        free(car);
        free(rgb);
        free(nor);
        free(pol);
        free(tex);
        free(val);
    }
};
typedef struct sc_scan SC_SCN;
#define SC_SCN_SIZE sizeof(SC_SCN)

void msgError  (std::string str, int cod=0, std::ostream& ost = std::cerr);
void msgWarning(std::string str, std::ostream& ost = std::cout);

void cleanScan(SC_SCN& scn);
void readScan (SC_SCN& scn, std::string str, std::ostream& ost = std::cout);
void writeScan(SC_SCN& scn, std::string str, std::ostream& ost = std::cout);

} // namespace ps

#endif // __cplusplus

#endif // LIBPOINTSCAN_CORE_H
