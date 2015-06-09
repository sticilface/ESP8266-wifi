#include "HsvColor.h"
#include <RgbColor.h>

RgbColor HsvColor::HsvBlend(RgbColor left, RgbColor right, uint8_t progress)
{
//return RgbColor( left.R + ((right.R - left.R) * progress / 255),
//		left.G + ((right.G - left.G) * progress / 255),
//		left.B + ((right.B - left.B) * progress / 255));
}

RgbColor HsvColor::HsvToRgb(HsvColor hsvcol) {
   
    RgbColor colour; 
    double r, g, b;

    int i = int(hsvcol.H * 6);
    double f = hsvcol.H * 6 - i;
    double p = hsvcol.V * (1 - hsvcol.S);
    double q = hsvcol.V * (1 - f * hsvcol.S);
    double t = hsvcol.V * (1 - (1 - f) * hsvcol.S);

    switch(i % 6){
        case 0: r = hsvcol.V, g = t, b = p; break;
        case 1: r = q, g = hsvcol.V, b = p; break;
        case 2: r = p, g = hsvcol.V, b = t; break;
        case 3: r = p, g = q, b = hsvcol.V; break;
        case 4: r = t, g = p, b = hsvcol.V; break;
        case 5: r = hsvcol.V, g = p, b = q; break;
    }

    colour.R = r * 255;
    colour.G = g * 255;
    colour.B = b * 255;

    return colour;
}

HsvColor HsvColor::RgbToHsv(RgbColor colour) {

    byte r = colour.R;
    byte g = colour.G;
    byte b = colour.B;

    double rd = (double) r/255;
    double gd = (double) g/255;
    double bd = (double) b/255;
    double max = max(rd, max(gd, bd)), min = min(rd,min(  gd, bd));
    double h, s, v = max;

    double d = max - min;
    s = max == 0 ? 0 : d / max;

    if (max == min) { 
        h = 0; // achromatic
    } else {
        if (max == rd) {
            h = (gd - bd) / d + (gd < bd ? 6 : 0);
        } else if (max == gd) {
            h = (bd - rd) / d + 2;
        } else if (max == bd) {
            h = (rd - gd) / d + 4;
        }
        h /= 6;
    }

    return HsvColor(h,s,v);  

}

// helper modules... these have to go in private declarations... NOT WORKING...  mmmm
/*
double threeway_max(double a, double b, double c) {
    return max(a, max(b, c));
}

double threeway_min(double a, double b, double c) {
    return min(a, min(b, c));
}


double map_double(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
*/
