#ifndef SYMBOLIBRE_GRAPHICAL_PARAMETERS
#define SYMBOLIBRE_GRAPHICAL_PARAMETERS

/* This header defines constants to handle rendering details such as the font
 * size, the space between two tokens, etc. All quantities are in number of pixels.*/

constexpr int FONT_SIZE = 10; /* Font size parameter. */
constexpr int INTERSPACE = FONT_SIZE / 3; /* space between two tokens */

constexpr int FRAC_UP_SPACE = 2; /* space between the numerator nottom and the fraction bar below */
constexpr int FRAC_DOWN_SPACE = 2; /* space between the denominator top and the fraction bar above  */
constexpr int FRAC_SPACE = 0; /* space at the right and the left of the fraction (outside) */

constexpr int PAREN_SPACE = 1; /* space at the right and the left of the parenthesis */

constexpr int ROOT_HSPACE = 2; /* The space between the root top bar and the expression below */
constexpr int ROOT_VSPACE = 2; /* The space on the right and left of the root, under the bar. */
constexpr int ROOT_WIDTH = 2 * FONT_SIZE / 3; /* The size of the root symbol √ */

#endif
