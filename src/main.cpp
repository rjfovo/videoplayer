#include "application.h"

int main(int argc, char *argv[])
{
    Application app(argc, argv, "cutefish-videoplayer");

    return app.run();
}
