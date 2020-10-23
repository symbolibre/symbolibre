#ifndef SYMBOLIBRE_GRAPHICAL_PARAMETERS
#define SYMBOLIBRE_GRAPHICAL_PARAMETERS

/* This header defines constants to handle rendering details such as the font
 * size, the space between two tokens, etc. All quantities are in number of pixels.*/

constexpr int INTERSPACE = 0; /* space between two tokens */

constexpr int FRAC_UP_SPACE = 2; /* space between the numerator nottom and the fraction bar below */
constexpr int FRAC_DOWN_SPACE = 2; /* space between the denominator top and the fraction bar above  */
constexpr int FRAC_SPACE = 4; /* space at the right and the left of the fraction (outside) */

constexpr int PAREN_SPACE = 0; /* space at the right and the left of the parenthesis */

constexpr int ROOT_HSPACE = 2; /* The space between the root top bar and the expression below */
constexpr int ROOT_VSPACE = 2; /* The space on the right and left of the root, under the bar. */

constexpr int SIGMA_DESCENT = 3; /* How much the sum symbol descends under the baseline */

constexpr int INT_OVERLAP = 4; /* How much the upper and lower bound of an integral overlap the integral symbol's height */
constexpr int INT_DESCENT = 3; /* How much the integral symbol descends under the baseline */

#endif
