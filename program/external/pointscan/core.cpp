#include "core.h"

namespace ps {

void msgError(std::string str, int cod, std::ostream& ost)
{
    ost << str << std::endl;
    exit(cod);
}
void msgWarning(std::string str, std::ostream& ost)
{
    ost << str << std::endl;
}

void cleanScan(SC_SCN& scn)
{
    if(scn.amp != 0)
    {
        free(scn.amp);
        scn.amp = 0;
    }
    if(scn.car != 0)
    {
        free(scn.car);
        scn.car = 0;
    }
    if(scn.rgb != 0)
    {
        free(scn.rgb);
        scn.rgb = 0;
    }
    if(scn.nor != 0)
    {
        free(scn.nor);
        scn.nor = 0;
    }
    if(scn.pol != 0)
    {
        free(scn.pol);
        scn.pol = 0;
    }
    if(scn.val != 0)
    {
        free(scn.val);
        scn.val = 0;
    }
    if(scn.tex != 0)
    {
        free(scn.tex);
        scn.tex = 0;
    }
}

void readScan(SC_SCN& scn, std::string str, std::ostream& ost)
{
    FILE *ifd;
  #pragma warning(disable: 4996)
    if (!(ifd = fopen(str.data(), "rb")))
        msgError("ReadScan: Cannot read " + str, 1, ost);
    if(fread(&scn.hdr, SC_HDR_SIZE, 1, ifd) != 1)
        msgError("ReadScan: Cannot read header of " + str, 1, ost);
  #pragma warning(default: 4996)

    int ele = scn.hdr.hgt * scn.hdr.wdt;
    for(int i = 0 ; i < (int)scn.hdr.cur ; i++)
    {
             if(scn.hdr.ord[i] == AMP)
        {
            if(!(scn.amp = (SC_AMP *)malloc(ele * SC_AMP_SIZE)))
                msgError("ReadScan: Cannot allocate data buffer", 1, ost);
            if(fread(scn.amp, SC_AMP_SIZE, ele, ifd) != ele)
                msgError("ReadScan: Cannot read amplitude data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == CAR)
        {
            if(!(scn.car = (SC_CAR *)malloc(ele * SC_CAR_SIZE)))
                msgError("ReadScan: Cannot allocate data buffer", 1, ost);
            if(fread(scn.car, SC_CAR_SIZE, ele, ifd) != ele)
                msgError("ReadScan: Cannot read data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == RGB)
        {
            if(!(scn.rgb = (SC_RGB *)malloc(ele * SC_RGB_SIZE)))
                msgError("ReadScan: Cannot allocate data buffer", 1, ost);
            if(fread(scn.rgb, SC_RGB_SIZE, ele, ifd) != ele)
                msgError("ReadScan: Cannot read data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == NOR)
        {
            if(!(scn.nor = (SC_NOR *)malloc(ele * SC_NOR_SIZE)))
                msgError("ReadScan: Cannot allocate data buffer", 1, ost);
            if(fread(scn.nor, SC_NOR_SIZE, ele, ifd) != ele)
                msgError("ReadScan: Cannot read data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == POL)
        {
            if(!(scn.pol = (SC_POL *)malloc(ele * SC_POL_SIZE)))
                msgError("ReadScan: Cannot allocate data buffer", 1, ost);
            if(fread(scn.pol, SC_POL_SIZE, ele, ifd) != ele)
                msgError("ReadScan: Cannot read data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == TEX)
        {
            if(!(scn.tex = (SC_TEX *)malloc(ele * SC_TEX_SIZE)))
                msgError("ReadScan: Cannot allocate data buffer", 1, ost);
            if(fread(scn.tex, SC_TEX_SIZE, ele, ifd) != ele)
                msgError("ReadScan: Cannot read data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == VAL)
        {
            if(!(scn.val = (SC_VAL *)malloc(ele * SC_VAL_SIZE)))
                msgError("ReadScan: Cannot allocate data buffer", 1, ost);
            if(fread(scn.val, SC_VAL_SIZE, ele, ifd) != ele)
                msgError("ReadScan: Cannot read validity data in " + str, 1, ost);
        }
    }
    if(fclose(ifd))
        msgError("ReadScan: Cannot close in " + str, 1, ost);
}

void writeScan(SC_SCN& scn, std::string str, std::ostream& ost)
{
    FILE *ofp;
  #pragma warning(disable: 4996)
    if(!(ofp = fopen(str.data(), "wb")))
        msgError("WriteScan: Cannot create " + str, 1, ost);
    if(fwrite(&scn.hdr, SC_HDR_SIZE, 1, ofp) != 1)
        msgError("WriteScan: Cannot write header in " + str, 1, ost);
  #pragma warning(default: 4996)

    int ele  = scn.hdr.hgt * scn.hdr.wdt;
    for(int i = 0 ; i < (int)scn.hdr.cur ; i++)
    {
             if(scn.hdr.ord[i] == AMP)
        {
            if(!scn.amp)
                msgError("WriteScan: Intensity data error", 1, ost);
            else
                if(fwrite(scn.amp, SC_AMP_SIZE, ele, ofp) != ele)
                    msgError("WriteScan: Cannot write intensity data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == CAR)
        {
            if(!scn.car)
                msgError("WriteScan: Cartesian coordinates data error", 1, ost);
            else
                if(fwrite(scn.car, SC_CAR_SIZE, ele, ofp) != ele)
                    msgError("WriteScan: Cannot write cartesian coordinates data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == RGB)
        {
            if(!scn.rgb)
                msgError("WriteScan: Color data error", 1, ost);
            else
                if(fwrite(scn.rgb, SC_RGB_SIZE, ele, ofp) != ele)
                    msgError("WriteScan: Cannot write color data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == NOR)
        {
            if(!scn.nor)
                msgError("WriteScan: Normals data error", 1, ost);
            else
                if(fwrite(scn.nor, SC_NOR_SIZE, ele, ofp) != ele)
                    msgError("WriteScan: Cannot write normals data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == POL)
        {
            if(!scn.pol)
                msgError("WriteScan: Polar coordinates data error", 1, ost);
            else
                if(fwrite(scn.pol, SC_POL_SIZE, ele, ofp) != ele)
                    msgError("WriteScan: Cannot write polar coordinates data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == TEX)
        {
            if(!scn.tex)
                msgError("WriteScan: Texture data error", 1, ost);
            else
                if(fwrite(scn.tex, SC_TEX_SIZE, ele, ofp) != ele)
                    msgError("WriteScan: Cannot write texture data in " + str, 1, ost);
        }
        else if(scn.hdr.ord[i] == VAL)
        {
            if(!scn.val)
                msgError("WriteScan: Validity data error", 1, ost);
            else
                if(fwrite(scn.val, SC_VAL_SIZE, ele, ofp) != ele)
                    msgError("WriteScan: Cannot write validity data in " + str, 1, ost);
        }
    }
    if(fclose(ofp))
        msgError("WriteScan: Cannot close in " + str, 1, ost);
}

} // namespace ps
