## Some informations about the code

	This is a very simple sketch of a pretty printer. The program
expects two strings as inputs, a [top] one and a [bottom] one, then beautifies
the fraction: `top / bottom`.

* Rendering such expressions is a first step - and an interesting one - because
you have to align two strings and draw a fraction bar of the correct size.
* The later will be a lot more complicated because it won't be restricted
on a single fraction bar.

# About `main.cpp`

	I expect Aymeric to produce a parser that will give me an AST associated
to some arithmetic expression. So, the program will await an input from the
user, then run the parser on it, get an AST, and beautify the expression using
it. The expected shape of `main.cpp` will be:

```cpp
int main(int argc, char* argv[])
{
	/* (...) (administrative part) */
	ASTree expression_tree = yyparse();
	QApplication app(argc, argv);
	Window window(expression_tree);
	window.show();
  return app.exec();
}
```

# Informations about QPaint

* I draw directly the expression in a Widget 'PrettyPrinter', then it is 
rendered by a 'Window' object. To draw, I use **QPaint**.


`drawLine(...)` : the name is self-explanatory
`drawText(...)` : same
`fontMetrics(...)`: ...


* To get informations about *font*, characters size, bounding rectangles
on strings, I use the **QFontMetrics**, **QRect** classes.

`boundingRect(...)` : ...
`getRect(...)` : ...

