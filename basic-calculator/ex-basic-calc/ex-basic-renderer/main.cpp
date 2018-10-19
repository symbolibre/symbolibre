#include "window.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Expected './pp [top:string] [bot:string]'\n");
        return 1;
    }
    QApplication app(argc, argv);
    Window window(argv[1], argv[2]);
    window.show();
    return app.exec();
}
