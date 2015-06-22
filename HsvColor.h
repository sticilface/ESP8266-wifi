




/*--------------------------------------------------------------------
NeoPixel is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.
NeoPixel is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
--------------------------------------------------------------------*/
#pragma once

#include <Arduino.h>
#include <RgbColor.h>
// ------------------------------------------------------------------------
// HsvColor represents a color object that is represented by Hue, Saturation, Value
// component values.  It contains helpful color routines to manipulate the 
// color.  Adapted from..
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


// ------------------------------------------------------------------------

/*
struct HsvColor
{
    // ------------------------------------------------------------------------
    // Construct HsvColor using H, S, V values (0-255)
    // ------------------------------------------------------------------------
	HsvColor(uint8_t h, uint8_t s, uint8_t v) :
		H(h), S(s), V(v)
	{
	};

    uint8_t H;
	uint8_t S;
	uint8_t V;
	};
*/

struct HsvColor
{
    // ------------------------------------------------------------------------
    // Construct HsvColor using H, S, V values (0-255)
    // ------------------------------------------------------------------------
	HsvColor(double h, double s, double v) :
		H(h), S(s), V(v)
	{
	};

	
	static RgbColor HsvBlend(RgbColor left, RgbColor right, uint8_t progress);
    
    static RgbColor HsvToRgb(HsvColor hsvcol); 

    static HsvColor RgbToHsv(RgbColor colour);

    // ------------------------------------------------------------------------
    // Hue, Sat, Value color members double where 
    // 
    // ------------------------------------------------------------------------
	


    double H;
	double S;
	double V;
	


	double threeway_max(double a, double b, double c) ;
	double threeway_min(double a, double b, double c) ;
	double map_double(double x, double in_min, double in_max, double out_min, double out_max); 
};

