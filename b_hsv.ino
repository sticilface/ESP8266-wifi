// CREDITS.....
/*
 * RGBConverter.h - Arduino library for converting between RGB, HSV and HSL
 * 
 * Ported from the Javascript at http://mjijackson.com/2008/02/rgb-to-hsl-and-rgb-to-hsv-color-model-conversion-algorithms-in-javascript
 * The hard work was Michael's, all the bugs are mine.
 *
 * Robert Atkins, December 2010 (ratkins_at_fastmail_dot_fm).
 *
 * https://github.com/ratkins/RGBConverter
 *
 */




//
// NOT IN USE
/*

void rgbToHsl(byte r, byte g, byte b, double hsl[]) { 
    double rd = (double) r/255;
    double gd = (double) g/255;
    double bd = (double) b/255;
    double max = threeway_max(rd, gd, bd);
    double min = threeway_min(rd, gd, bd);
    double h, s, l = (max + min) / 2;

    if (max == min) {
        h = s = 0; // achromatic
    } else {
        double d = max - min;
        s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
        if (max == rd) {
            h = (gd - bd) / d + (gd < bd ? 6 : 0);
        } else if (max == gd) {
            h = (bd - rd) / d + 2;
        } else if (max == bd) {
            h = (rd - gd) / d + 4;
        }
        h /= 6;
    }
    hsl[0] = h;
    hsl[1] = s;
    hsl[2] = l;
}

*/

/**
 * Converts an HSL color value to RGB. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSL_color_space.
 * Assumes h, s, and l are contained in the set [0, 1] and
 * returns r, g, and b in the set [0, 255].
 *
 * @param   Number  h       The hue
 * @param   Number  s       The saturation
 * @param   Number  l       The lightness
 * @return  Array           The RGB representation
 */

/*

void hslToRgb(double h, double s, double l, byte rgb[]) {
    double r, g, b;

    if (s == 0) {
        r = g = b = l; // achromatic
    } else {
        double q = l < 0.5 ? l * (1 + s) : l + s - l * s;
        double p = 2 * l - q;
        r = RGBConverter::hue2rgb(p, q, h + 1/3);
        g = RGBConverter::hue2rgb(p, q, h);
        b = RGBConverter::hue2rgb(p, q, h - 1/3);
    }

    rgb[0] = r * 255;
    rgb[1] = g * 255;
    rgb[2] = b * 255;
}

*/ 

/**
 * Converts an RGB color value to HSV. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSV_color_space.
 * Assumes r, g, and b are contained in the set [0, 255] and
 * returns h, s, and v in the set [0, 1].
 *
 * @param   Number  r       The red color value
 * @param   Number  g       The green color value
 * @param   Number  b       The blue color value
 * @return  Array           The HSV representation
 */


//void rgbToHsv(byte r, byte g, byte b, double hsv[]) {

RgbColor rgbToHsv(RgbColor colour) {

    byte r = colour.R;
    byte g = colour.G;
    byte b = colour.B;

    double rd = (double) r/255;
    double gd = (double) g/255;
    double bd = (double) b/255;
    double max = threeway_max(rd, gd, bd), min = threeway_min(rd, gd, bd);
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

    colour.R = h * 255 ;
    colour.G = s * 255 ;
    colour.B = v * 255 ;

return colour; 

}


/**
 * Converts an HSV color value to RGB. Conversion formula
 * adapted from http://en.wikipedia.org/wiki/HSV_color_space.
 * Assumes h, s, and v are contained in the set [0, 1] and
 * returns r, g, and b in the set [0, 255].
 *
 * @param   Number  h       The hue
 * @param   Number  s       The saturation
 * @param   Number  v       The value
 * @return  Array           The RGB representation
 */
//void hsvToRgb(double h, double s, double v, byte rgb[]) {

RgbColor hsvToRgb(double h, double s, double v) {
   
    RgbColor colour; 
    double r, g, b;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    colour.R = r * 255;
    colour.G = g * 255;
    colour.B = b * 255;

    return colour;
}
 
double threeway_max(double a, double b, double c) {
    return max(a, max(b, c));
}

double threeway_min(double a, double b, double c) {
    return min(a, min(b, c));
}

double hue2rgb(double p, double q, double t) {
    if(t < 0) t += 1;
    if(t > 1) t -= 1;
    if(t < 1/6) return p + (q - p) * 6 * t;
    if(t < 1/2) return q;
    if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
    return p;
}


RgbColor dimbyhsv(RgbColor color, byte dim) {

// takes RGB ------- > HSV

    //byte r = color.R;
    //byte g = color.G;
    //byte b = color.B;

    double rd = (double) color.R/255;
    double gd = (double) color.G/255;
    double bd = (double) color.B/255;
    double max = threeway_max(rd, gd, bd), min = threeway_min(rd, gd, bd);
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

    v = (double) dim/255; 

//////  takes HSV -----> RGB

    double rD, gD, bD;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: rD = v, gD = t, bD = p; break;
        case 1: rD = q, gD = v, bD = p; break;
        case 2: rD = p, gD = v, bD = t; break;
        case 3: rD = p, gD = q, bD = v; break;
        case 4: rD = t, gD = p, bD = v; break;
        case 5: rD = v, gD = p, bD = q; break;
    }

    color.R = rD * 255;
    color.G = gD * 255;
    color.B = bD * 255;

return color;


}



RgbColor Hsv_COLOR_range_rgb(RgbColor color1, byte lower, byte upper, byte progress) {


    double r1d = (double) color1.R/255;
    double g1d = (double) color1.G/255;
    double b1d = (double) color1.B/255;
    double max = threeway_max(r1d, g1d, b1d), min = threeway_min(r1d, g1d, b1d);
    double h1, s1, v1 = max;

    double d1 = max - min;
    s1 = max == 0 ? 0 : d1 / max;

    if (max == min) { 
        h1 = 0; // achromatic
    } else {
        if (max == r1d) {
            h1 = (g1d - b1d) / d1 + (g1d < b1d ? 6 : 0);
        } else if (max == g1d) {
            h1 = (b1d - r1d) / d1 + 2;
        } else if (max == b1d) {
            h1 = (r1d - g1d) / d1 + 4;
        }
        h1 /= 6;
    }

double low = h1 - (double) lower/360;
double high = h1 + (double) upper/360;

if (low < 0) low += 1; 
if (high > 1) high -= 1; 


double h = map_double ((double) progress, 0, 255, low,high);
double s = s1, v = v1; 

RgbColor color; 

    double rD, gD, bD;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: rD = v, gD = t, bD = p; break;
        case 1: rD = q, gD = v, bD = p; break;
        case 2: rD = p, gD = v, bD = t; break;
        case 3: rD = p, gD = q, bD = v; break;
        case 4: rD = t, gD = p, bD = v; break;
        case 5: rD = v, gD = p, bD = q; break;
    }

    color.R = rD * 255;
    color.G = gD * 255;
    color.B = bD * 255;

return color;


}




RgbColor HsvFADErgb(RgbColor color1, RgbColor color2, byte progress) {


// COLOUR 1  ----->  HSV

    double r1d = (double) color1.R/255;
    double g1d = (double) color1.G/255;
    double b1d = (double) color1.B/255;
    double max = threeway_max(r1d, g1d, b1d), min = threeway_min(r1d, g1d, b1d);
    double h1, s1, v1 = max;

    double d1 = max - min;
    s1 = max == 0 ? 0 : d1 / max;

    if (max == min) { 
        h1 = 0; // achromatic
    } else {
        if (max == r1d) {
            h1 = (g1d - b1d) / d1 + (g1d < b1d ? 6 : 0);
        } else if (max == g1d) {
            h1 = (b1d - r1d) / d1 + 2;
        } else if (max == b1d) {
            h1 = (r1d - g1d) / d1 + 4;
        }
        h1 /= 6;
    }


// COLOUR 2  ----->  HSV

    double r2d = (double) color2.R/255;
    double g2d = (double) color2.G/255;
    double b2d = (double) color2.B/255;
    max = threeway_max(r2d, g2d, b2d);
    min = threeway_min(r2d, g2d, b2d);
    double h2, s2, v2 = max;

    double d2 = max - min;
    s2 = max == 0 ? 0 : d2 / max;

    if (max == min) { 
        h2 = 0; // achromatic
    } else {
        if (max == r2d) {
            h2 = (g2d - b2d) / d2 + (g2d < b2d ? 6 : 0);
        } else if (max == g2d) {
            h2 = (b2d - r2d) / d2 + 2;
        } else if (max == b2d) {
            h2 = (r2d - g2d) / d2 + 4;
        }
        h2 /= 6;
    }
/*
Serial.println();
Serial.print("Colour 1: ("); Serial.print(color1.R); Serial.print(",") ; Serial.print(color1.G) ; Serial.print(",") ; Serial.print(color1.B) ; Serial.print(") ") ;
Serial.print("-->");
Serial.print("("); Serial.print(h1); Serial.print(",") ; Serial.print(s1) ; Serial.print(",") ; Serial.print(v1) ; Serial.print(") ") ;

// colour 2..

Serial.println();
Serial.print("Colour 2: ("); Serial.print(color2.R); Serial.print(",") ; Serial.print(color2.G) ; Serial.print(",") ; Serial.print(color2.B) ; Serial.print(") ") ;
Serial.print("-->");
Serial.print("("); Serial.print(h2); Serial.print(",") ; Serial.print(s2) ; Serial.print(",") ; Serial.print(v2) ; Serial.print(") ") ;

*/

double h = map_double ((double) progress, 0, 255, h1,h2);
double v = map_double ((double) progress, 0, 255, v1,v2);
double s = map_double ((double) progress, 0, 255, s1,s2);

/*
Serial.println();
Serial.print(progress);
Serial.print(" Hue: ");
Serial.print(h1);
Serial.print(" + ");
Serial.print(h2);
Serial.print(" = ");
Serial.print(huemap);
Serial.print(" Value: ");
Serial.print(v1);
Serial.print(" + ");
Serial.print(v2);
Serial.print(" = ");
Serial.print(valuemap);

*/

//////  takes HSV -----> RGB

RgbColor color; 

    double rD, gD, bD;

    int i = int(h * 6);
    double f = h * 6 - i;
    double p = v * (1 - s);
    double q = v * (1 - f * s);
    double t = v * (1 - (1 - f) * s);

    switch(i % 6){
        case 0: rD = v, gD = t, bD = p; break;
        case 1: rD = q, gD = v, bD = p; break;
        case 2: rD = p, gD = v, bD = t; break;
        case 3: rD = p, gD = q, bD = v; break;
        case 4: rD = t, gD = p, bD = v; break;
        case 5: rD = v, gD = p, bD = q; break;
    }

    color.R = rD * 255;
    color.G = gD * 255;
    color.B = bD * 255;

return color;









} // end of HsvFADErgb



double map_double(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

