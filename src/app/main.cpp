#include <QApplication>
#include "ui/MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("VideoEditor");
    app.setOrganizationName("OpenSource");

    ve::ui::MainWindow window;
    window.show();

    return app.exec();
}
