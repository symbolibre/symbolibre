### Symbolibre IDE

This repo. stored the IDE designed for the Symbolibre Project.


 TODO :
* Fix the "language choice" when starting the IDE :
- The user MUST declare the file type ( .txt, .py, .ml ... ) to use the IDE. It will allow us to use the appropriate syntax highlighter/autocompletion/'blocks' ( while, if/then/else ... with the good syntax, according to the language ).  
- Make the popup 'pop' as soon as the IDE is started. We use a popup with buttons corresponding to languages to make it easier to use for non-programmers, to avoid problems coming from bad extension/file name.

* Fix the 'save' problem : for the moment, saving a file that has never been saved ( with 'Save' instead of 'Save as ...' ) results in an error, as we do not know where to store it.
- Call 'Save as ...' instead of 'Save' when the file has just been created
- Or make the user decide at the very beginning ( i.e. before even deciding the 'language' in which he will code ) where his program will be stored.

Ideas to store the language : add an attribute to the 'Document' class ( already - very - partially done ), or simply use a function to detect/modify the file extension.

* Syntax highlighter

* Make the IDE pretty ~ Use icons for the 'Save'/'Open'... buttons, make menus, allow the user to modify the font size, to use bold/underline/italic functions in .txt files ( The functions are already done - see documenthandler.cpp for more information ).
